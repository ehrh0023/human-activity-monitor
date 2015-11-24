#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "RegionFinder.h"
#include "ColorProfiler.h"
#include "StatsGenerator.h"

/**
 * @class HandTracker
 * @brief Main application class for tracking hands
 */
class HandTracker
{
public:
	/**
	 * Constructor from a camera ID
	 * @param id of the camera to use
	 */
	HandTracker(int cam_id = 0);

	/**
	 * Constructor from file name
	 * @param name of the file to load
	 */
	HandTracker(std::string file_name);

	/**
	 * Switched video source to a new camera
	 * @param id of the camera to use
	 */
	void switch_source(int cam_id);

	/**
	 * Switched video source to a new file
	 * @param name of the file to load
	 */
	void switch_source(std::string file_name);

	/**
	 * Get a new frame and process it
	 * @return the frame processed
	 */
	MovementSample process(cv::Mat frame);

	/**
	 * Repeatedly call update
	 * @return success code
	 */
	int run();
	
	cv::Mat next_frame();

	bool is_file();
	bool is_cam();

	void restart_video();

	void set_stats_file(std::string);
	std::string get_stats_file();

	cv::Size capture_size();
private:
	cv::VideoCapture cap;
	RegionFinder regionFinder;

	StatsGenerator stats;

	bool loadedfromfile;
};