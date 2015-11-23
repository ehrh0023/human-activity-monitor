#include "HandFinder.h"
#include "Region.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>





using namespace cv;
using namespace std;
using namespace cv;

float HandFinder::distance_thresh = 300;

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
	float second_largest = -1;
	Region largesthand;
	Region secondlargesthand;

	for (int j = 0; j < possible.size(); j++)
	{
		Region possible_region = possible[j];
		//find area of the regions
		float hands = possible_region.moment.m00;

		if (hands > largest)
		{
			second_largest = largest;
			secondlargesthand = largesthand;
			largest = hands;
			largesthand = possible_region;
		}
		else if (hands > second_largest)
			second_largest = hands;
			secondlargesthand = possible_region;
	}
	
	std::vector<Region> data;
	data.push_back(largesthand);
	data.push_back(largest_region);
	data.push_back(secondlargesthand);

	return data;
}