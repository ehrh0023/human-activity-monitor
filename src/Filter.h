#pragma once
#include <opencv2/opencv.hpp>
#include <string>

/**
 * @class Filter
 * @brief An abstract class used to attach multiple types of filters to a process
 */
struct Filter
{
	/**
	 * Filter the image
	 * @param window The image to filter
	 * @return the filtered image
	 */
	virtual cv::Mat filter(cv::Mat image) = 0;
};

/**
 * @class HSVFilter
 * @brief A Filter which filters out colors outside of a specific HSV bound
 */
struct HSVFilter : public Filter
{
	int low_hue;  /** Lower bound of the hue to keep */
	int high_hue; /** Upper bound of the hue to keep */
	int low_sat;  /** Lower bound of the saturation to keep */
	int high_sat; /** Upper bound of the saturation to keep */
	int low_val;  /** Lower bound of the value to keep */
	int high_val; /** Upper bound of the value to keep */

	HSVFilter();
	~HSVFilter();

	/**
	 * Filter the image via HSV
	 * @param window The image to filter
	 * @return the HSV filtered image
	 */
	cv::Mat filter(cv::Mat image);
};