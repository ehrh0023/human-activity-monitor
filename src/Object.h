#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

struct Object
{
	float mass;
	cv::Moments moments;
	cv::Point2f center;
	std::vector<cv::Point> contour;
	cv::Vec4i hierarchy;

	static cv::RNG rng;

	void draw(cv::Mat window);
};