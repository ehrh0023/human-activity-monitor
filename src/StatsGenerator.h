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
    std::chrono::duration<double> time = std::chrono::system_clock::duration::zero();
	double velocity = 0;
	double frequency = 0;
};


/**
 * @class StatsGenerator
 * @brief Creates frequency statistics between frame movements 
 */
class StatsGenerator
{
public:

	/**
	 *  Create the generator without saving abilities
	 */
	StatsGenerator();

	/**
	 *  Create the generator
	 * @param file_name to save to
	 */
	StatsGenerator(std::string file_name);

	/**
	 * Add a new sample
	 * @param regions of interest
	 * @param save file
	 */
    MovementSample create_sample(HandInfo info, bool save = false);
    
    MovementSample oscillation_detection(MovementSample sample, cv::Point point, cv::Point lastPoint, bool save);
    
	void save_sample(MovementSample sample);
    
	void set_save_file(std::string filename);
	std::string get_save_file();


private:
  
	double displacement = 0;
	int State = 0;   // 1 = UP; 0 = DOWN;
	cv::Point lastCenter;
	bool drop = true;
	double max_height = 0;
    double min_height = 0;
    double dist_thresh = 1;
		
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> AlgStartTime;
	std::chrono::duration<double> decay_time = std::chrono::system_clock::duration::zero();
	
	std::string file_path;
	bool savable;
};