#pragma once
#include <vector>
#include "Filter.h"
#include "Object.h"

struct ObjectFinder
{
	int thresh;   // How well does it detect edges
	static const int max_thresh;   // Maximum amount of threshold

	ObjectFinder(int threshold = 100);

	~ObjectFinder();

	std::vector<Filter*> filters;

	std::vector<Object> find(cv::Mat image);

};
