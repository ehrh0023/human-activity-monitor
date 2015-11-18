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
	/**
	 *  Create the generator
	 * @param file_name to save to
	 */
	StatsGenerator(std::string file_name = "Data.csv");

	/**
	 * Add a new sample
	 * @param frame to add
	 * @param regions of interest
	 */
	std::vector<Region> add_sample(cv::Mat frame, std::vector<Region>& detectedObj);//cv::Point sample);
	
	int frames = 0;
	bool cycle;
	bool cycle2;
	double unitTime = 1/30;
    int Cycles = 0;
    int cycFrames = 0;
	cv::Point handCenterLast;
private:
	std::string file_path;
	
};