#pragma once
#include <opencv2/opencv.hpp>
#include "HSVRange.h"

class ColorProfiler
{
	cv::CascadeClassifier face_cascade;

public:

	ColorProfiler(std::string const& filename);

	HSVRange determine_colors(cv::Mat img);
};