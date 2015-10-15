#include "Region.h"

using namespace cv;

RNG Region::rng(12345);

void Region::draw(cv::Mat window)
{
	// Draw the contour and center with a random color
	Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	polylines(window, contour, true, color, 1, 8);
	circle(window, center, 4, color, -1, 8, 0);
}
