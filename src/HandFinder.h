#include <opencv2/opencv.hpp>
#include <utility>
#include "Region.h"

namespace HandFinder
{
	std::vector<Region> find_hands(cv::Mat image,const std::vector<Region>& regions);
};