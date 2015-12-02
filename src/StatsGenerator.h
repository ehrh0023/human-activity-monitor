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
    double time = 0;
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
    /**
	 * Extract sample metrics
	 * @param created sample
	 * @param current center point of hands
	 * @param last center point of hands
	 * @param save file
	 */
    MovementSample oscillation_detection(MovementSample sample, cv::Point point, cv::Point lastPoint, bool save);

	void save_sample(MovementSample sample);
	void set_save_file(std::string filename);
	std::string get_save_file();
private:
	int State = 0;             // 1 = UP; 0 = DOWN;
	cv::Point2f lastCenter;      // Save last center point for diplacement calculation
	bool drop = true;          // drop flag; drop frame if above head plus threshold
	double max_height = 0;     // maximum height amplitude
    double min_height = 0;     // minimum height amplitude
    double dist_thresh = 1;    // distance threshold; threshold to be considered a hand flap
	double displacement = 0;   // displacement between current y point and last y point 	
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start;          // start of cycle time
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> AlgStartTime;   // start of algorithm time 
	const double clock_delta = 0.000000001;

	std::string file_path;
	bool savable;
};
