#include "HandFinder.h"
#include "Region.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <utility>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

float HandFinder::distance_thresh = 300;
float HandFinder::hand_thresh = 180;

//calculate the distance between two regions
double distancebetweenpoint(Point c, Point d) 
{
	int X_dist = c.x - d.x;
	int Y_dist = c.y - d.y;
	return sqrt((X_dist*X_dist) + (Y_dist * Y_dist));

}

HandInfo HandFinder::find_hands(cv::Mat frame, const std::vector<Region>& regions)
{
	HandInfo info;

	double largest_area = -1;
	Region head;
	//go through all of the regions
	for (size_t i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];

		//find area of the regions
		double area = regions[i].moment.m00;
		if (area > largest_area)
		{
			largest_area = area;
			head = region; // lable the largest region of the regions "head"
			//Rect bounding_rect = boundingRect(regions[i].contour);
		}
	}
	if (largest_area == -1)
	{
		return info;
	}

	std::vector<Region> region_thresh = find_within(distance_thresh, head.center, regions);
	if(region_thresh.size() == 0)
	{
    	return info;
	}

	std::vector<Region> left_half = get_left_regions(head.center, region_thresh); // get the regions on the left side of the middle vertical line of the frame
	std::vector<Region> right_half = get_right_regions(head.center, region_thresh); // get the regions on the right side of the middle vertical line of the frame
	if (left_half.empty() || right_half.empty())
	{
		return info;
	}

	largest_area = -1;
	std::pair<Region, Region> hand_pair;
	for (size_t i = 0; i < region_thresh.size(); i++)
	{
		Region region = region_thresh[i];

		std::vector<Region> otherside;
		if (region.center.x < head.center.x) // if the center of the region is less than the head's center, then the region is on the right half
		{
			otherside = right_half;
		}
		else
		{
			otherside = left_half; // if the center of the region is greater than the head's center, then the region is on the left half
		}

		for (size_t j = 0; j < otherside.size(); j++)
		{ 
			Region other = otherside[j];
			double combined_area = region.moment.m00 + other.moment.m00; // adding two areas
			if (combined_area > largest_area)
			{
				largest_area = combined_area;
				hand_pair.first = region;
				hand_pair.second = other;
			}
		}	
	}
    
	if (largest_area == -1)
	{
		return info;
	}

	info.left_hand = hand_pair.first;
	info.head = head;
	info.right_hand = hand_pair.second;
	info.success = true;

	return info;
}

std::vector<Region> HandFinder::get_left_regions(cv::Point2f midpoint, const std::vector<Region>& regions)
{
	std::vector<Region> left_regions;

	for (size_t i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];
		if (region.center.x < midpoint.x)
		{
			left_regions.push_back(region); // if the region's center is less than the midpoint, so then set that region is left-regions
		}

	}

	return left_regions;
}

std::vector<Region> HandFinder::get_right_regions(cv::Point2f midpoint, const std::vector<Region>& regions)
{
	std::vector<Region> right_regions;

	for (size_t i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];
		if (region.center.x > midpoint.x)
		{
			right_regions.push_back(region); // if the region's center is greater than the midpoint, then set that region as the right_region. 
		}

	}

	return right_regions;
}

std::vector<Region> HandFinder::find_within(float distance, cv::Point2f point, const std::vector<Region>& regions)
{
	std::vector<Region> region_thresh;
	for (size_t i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];

		if (region.center == point) // if the region's center equals to head's center, then continue
			continue;

		double dist = distancebetweenpoint(point, region.center); //calculating the distance between the regions' centers
		if (dist <= distance_thresh)
		{
			region_thresh.push_back(region); // if the distance between the center of the region with the head's center is less or equal to thresh, set that region as region_thresh (those are the regions that possibly are hands regions)
		}
	}
	return region_thresh;
}
