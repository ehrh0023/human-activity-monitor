#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "Region.h"
#include "ColorProfiler.h"
#include <chrono>
#include <ctime>
#include "HandFinder.h"

struct MovementSample
{
	float time;
	float velocity;
	float frequency;
};


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
	MovementSample create_sample(cv::Mat frame, HandInfo info, bool save = false);//cv::Point sample);

	void save_sample(MovementSample sample);

private:
	int frames = 0;
	int cycles = 0;
	double distance = 0;
	int State = 0;
	cv::Point handCenterLast;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start, end;
	std::chrono::duration<double> cycleTime;     // Contains the time duration of a cycle
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> CycleCompTime;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> AlgStartTime;
	std::chrono::duration<double> cycleOccurTime;     // Contains the time duration of a cycle

	std::string file_path;
};