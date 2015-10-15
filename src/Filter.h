#pragma once
#include <opencv2/opencv.hpp>
#include <string>

struct Filter
{
	virtual cv::Mat filter(cv::Mat image) = 0;
};

struct HSVFilter : public Filter
{
	int iLowH;
	int iHighH;
	int iLowS;
	int iHighS;
	int iLowV;
	int iHighV;

	HSVFilter();

	~HSVFilter();

	cv::Mat filter(cv::Mat image);
};