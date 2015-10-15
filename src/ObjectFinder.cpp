#include "ObjectFinder.h"

using namespace cv;
using namespace std;

const int ObjectFinder::max_thresh = 255;

ObjectFinder::ObjectFinder(int threshold /*= 100*/) 
	:thresh(threshold)
{

}

ObjectFinder::~ObjectFinder()
{
	for (int i = 0; i < filters.size(); i++)
	{
		delete filters[i];
	}
}

std::vector<Object> ObjectFinder::find(Mat image)
{
	Mat filtered_img = image;

	for (Filter* filter : filters)
	{
		filtered_img = filter->filter(filtered_img);
	}

	Mat canny_output;
	// Edge Detection
	Canny(filtered_img, canny_output, thresh, thresh * 2, 3);

	// Vectors
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Object> objects;

	// Find contours
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Get the moments
	vector<Moments> mu(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		Object object;
		object.contour = contours[i];
		object.hierarchy = hierarchy[i];
		object.moments = moments(contours[i], false);
		object.center = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
		objects.push_back(object);
	}

	return objects;
}