#pragma once
#include <opencv2/opencv.hpp>
#include <utility>
#include "Region.h"

struct HandInfo
{
	Region head;
	Region right_hand;
	Region left_hand;
	bool success;

	HandInfo() : success(false) {}

};

class HandFinder
{
public:
	static float hand_thresh;
	static float distance_thresh;
	static HandInfo find_hands(cv::Mat image, const std::vector<Region>& regions);

private:
	static std::vector<Region> get_left_regions(cv::Point2f midpoint, const std::vector<Region>& regions);
	static std::vector<Region> get_right_regions(cv::Point2f midpoint, const std::vector<Region>& regions);
	static std::vector<Region> find_within(float distance, cv::Point2f point, const std::vector<Region>& regions);
};