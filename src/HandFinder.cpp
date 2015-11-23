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





using namespace cv;
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

std::vector<Region> HandFinder::find_hands(cv::Mat frame, const std::vector<Region>& regions)
{
	float largest = -1;
	Region largest_region;
	std::vector<Region> possible;
	int largest_index = 0;
	//go through all of the regions
	for (int i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];
		//find area of the regions
		float head = regions[i].moment.m00;
		if (head > largest)
		{
			largest = head;
			largest_region = region;
			//Rect bounding_rect = boundingRect(regions[i].contour);
		}
	}
	for (int i = 0; i < regions.size(); i++)
	{
		Region region = regions[i];

		if (region.center == largest_region.center)
			continue;

		double dist = distancebetweenpoint(largest_region.center, region.center);
		if (dist <= distance_thresh)
		{
			possible.push_back(region);
		}
	}
	largest = -1;
	std::pair<Region, Region> largesthand;
	
	for (int j = 0; j < possible.size(); j++)
	{
		Region possible_region = possible[j];
		//find area of the regions		
		std::vector<Region> otherside;
		if (possible_region.center.x < largest_region.center.x)
		{
			otherside = get_right_regions(largest_region.center, possible);
		}
		else
		{
			otherside = get_left_regions(largest_region.center, possible);
		}


		for (int k = 0; k < otherside.size(); k++)
		{
			Region other = otherside[j];
			float handarea = possible_region.moment.m00 + other.moment.m00;
			if (handarea > largest)
			{
				largest = handarea;
				largesthand.first = possible_region;
				largesthand.second = other;

			}

		}
	}
	
	std::vector<Region> data;
	data.push_back(largesthand.first);
	data.push_back(largest_region);
	data.push_back(largesthand.second);

	return data;
}

std::vector<Region>  HandFinder::get_left_regions(cv::Point midpoint, std::vector<Region>& regions)
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

std::vector<Region>  HandFinder::get_right_regions(cv::Point midpoint, std::vector<Region>& regions)
{
	std::vector<Region> right_regions;

	for (int i = 0; i < right_regions.size(); i++)
	{
		Region region = regions[i];
		if (region.center.x > midpoint.x)
		{
			right_regions.push_back(region);
		}

	}

	return right_regions;
}