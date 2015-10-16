#include "RegionFinder.h"

using namespace cv;
using namespace std;

const int RegionFinder::max_thresh = 255;

RegionFinder::RegionFinder(int threshold /*= 100*/) 
	:thresh(threshold)
{

}

RegionFinder::~RegionFinder()
{
	for (int i = 0; i < filters.size(); i++)
	{
		delete filters[i];
	}
}

std::vector<Region> RegionFinder::find(Mat image)
{
	// Filter the image
	Mat filtered_img = image;
	for (Filter* filter : filters)
	{
		filtered_img = filter->filter(filtered_img);
	}

	// Edge Detection
	Mat canny_output;
	Canny(filtered_img, canny_output, thresh, thresh * 2, 3);

	// Find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Determine the Regions
	vector<Region> regions(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		Region object;
		object.contour = contours[i];
		object.moment = moments(contours[i], false);
		object.center = Point2f(static_cast<float>(object.moment.m10 / object.moment.m00), 
									static_cast<float>(object.moment.m01 / object.moment.m00));
		regions.push_back(object);
	}

	return regions;
}

void RegionFinder::add_filter(Filter* filter)
{
	filters.push_back(filter);
}
