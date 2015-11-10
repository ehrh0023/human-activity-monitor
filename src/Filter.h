#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include "HSVRange.h"
#include "NaiveBayesClassifier.h"

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
	HSVRange passband; /** Range of HSV values to keep */

	HSVFilter();
	~HSVFilter();

	/**
	 * Filter the image via HSV
	 * @param window The image to filter
	 * @return the HSV filtered image
	 */
	cv::Mat filter(cv::Mat image);
};

class BayesFilter : public Filter
{
	NaiveBayesClassifier bayes;

public:
	BayesFilter(std::string filename = "../assets/BayesPresetXYZ");

	cv::Mat filter(cv::Mat image);
};