#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

class Region
{
	static cv::RNG rng; /** OpenCV RNG generator for color */

public:
	cv::Moments moment; /** Moment of the region */
	cv::Point2f center; /** Center of mass of the region */
	std::vector<cv::Point> contour; /** The points that make up the region's contour */

	/**
	 * Draw this object
	 * @param window The image to draw on
	 */
	void draw(cv::Mat window);
};