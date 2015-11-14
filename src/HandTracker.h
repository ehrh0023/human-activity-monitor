#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "RegionFinder.h"
#include "ColorProfiler.h"
#include "StatsGenerator.h"

class HandTracker
{
public:
	HandTracker(int cam_id = 0);
	HandTracker(std::string file_name);
	~HandTracker();

	void switch_source(int cam_id);
	void switch_source(std::string file_name);
	cv::Mat update();
	int  run();
	
private:
	cv::VideoCapture cap;
	RegionFinder regionFinder;
	bool first;

	NaiveBayesClassifier classifier;
	StatsGenerator stats;
};