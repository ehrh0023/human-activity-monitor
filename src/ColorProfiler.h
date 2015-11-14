#pragma once
#include <opencv2/opencv.hpp>
#include "HSVRange.h"

/**
 * @class ColorProfiler
 * @brief Creates an HSV color profile using Haar Cascades of a sample image
 */
class ColorProfiler
{
	cv::CascadeClassifier face_cascade;

public:

	/**
	 * Create the color profiler
	 * @param filename of the haar cascade to use
	*/
	ColorProfiler(std::string const& filename);

	/**
	 * Determine the color range from an image
	 * @param img to take color info from
	 * @return the determined color range
	 */
	HSVRange determine_colors(cv::Mat img);
};