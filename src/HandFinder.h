#pragma once
#include <opencv2/opencv.hpp>
#include <utility>
#include "Region.h"

class HandFinder
{
public:
	static float distance_thresh;
	static std::vector<Region> find_hands(cv::Mat image,const std::vector<Region>& regions);
};