#include <opencv2/opencv.hpp>
#include <utility>
#include "Region.h"

namespace HandFinder
{
	cv::Point find_hands(const std::vector<Region>& regions);
};