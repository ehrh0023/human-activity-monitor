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

double distancebetweenpoint(Point c, Point d)
{
	int X_dist = c.x - d.x;
	int Y_dist = c.y - d.y;
	return sqrt((X_dist*X_dist) + (Y_dist * Y_dist));

}

HandInfo HandFinder::find_hands(cv::Mat frame, const std::vector<Region>& regions)
{
	float largest_area = -1;
	Region head;
	
	//go through all of the regions
	for (int i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];

		//find area of the regions
		float area = regions[i].moment.m00;
		if (area > largest_area)
		{
			largest_area = area;
			head = region;
			//Rect bounding_rect = boundingRect(regions[i].contour);
		}
	}

	std::vector<Region> region_thresh = find_within(distance_thresh, head.center, regions);

	largest_area = -1;
	std::pair<Region, Region> hand_pair;
	std::vector<Region> left_half = get_left_regions(head.center, region_thresh);
	std::vector<Region> right_half = get_right_regions(head.center, region_thresh);


	HandInfo info;
	info.success = false;
	for (int i = 0; i < region_thresh.size(); i++)
	{
		Region region = region_thresh[i];

		std::vector<Region> otherside;
		if (region.center.x < head.center.x)
		{
			otherside = right_half;
		}
		else
		{
			otherside = left_half;
		}

		for (int j = 0; j < otherside.size(); j++)
		{
			Region other = otherside[j];
			float combined_area = region.moment.m00 + other.moment.m00;
			if (combined_area > largest_area)
			{
				info.success = true;
				largest_area = combined_area;
				hand_pair.first = region;
				hand_pair.second = other;
			}
		}
	}
	
	info.left_hand = hand_pair.first;
	info.head =  head;
	info.right_hand = hand_pair.second;
	return info;
}

std::vector<Region> HandFinder::get_left_regions(cv::Point2f midpoint, const std::vector<Region>& regions)
{
	std::vector<Region> left_regions;

	for (int i = 0; i < left_regions.size(); i++)
	{
		Region region = regions[i];
		if (region.center.x < midpoint.x)
		{
			left_regions.push_back(region);
		}

	}

	return left_regions;
}

std::vector<Region> HandFinder::get_right_regions(cv::Point2f midpoint, const std::vector<Region>& regions)
{
	std::vector<Region> right_regions;

	for (int i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];
		if (region.center.x > midpoint.x)
		{
			right_regions.push_back(region);
		}

	}

	return right_regions;
}

std::vector<Region> HandFinder::find_within(float distance, cv::Point2f point, const std::vector<Region>& regions)
{
	std::vector<Region> region_thresh;
	for (int i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];

		if (region.center == point)
			continue;

		double dist = distancebetweenpoint(point, region.center);
		if (dist <= distance_thresh)
		{
			region_thresh.push_back(region);
		}
	}
	return region_thresh;
}
