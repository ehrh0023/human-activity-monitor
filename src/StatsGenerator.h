#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "Region.h"
#include "ColorProfiler.h"

/**
 * @class StatsGenerator
 * @brief Creates frequency statistics between frame movements 
 */
class StatsGenerator
{
public:
	StatsGenerator(std::string file_name = "Data.csv");

	void update(cv::Mat const& frame, const std::vector<Region>& regions);
	
	double distance;
	int frames;
private:
	std::string file_path;
};