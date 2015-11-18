#include "RegionFinder.h"

using namespace cv;
using namespace std;

const int RegionFinder::max_thresh = 255;

RegionFinder::RegionFinder(int threshold /*= 100*/) 
	:thresh(threshold)
{
	// Initialize the classifier
	classifier.load("../assets/BayesPresetXYZ");
}

RegionFinder::~RegionFinder()
{
	for (int i = 0; i < filters.size(); i++)
	{
		delete filters[i];
	}
}

void RegionFinder::find(Mat image, std::vector<Region>& regions)
{
	// Filter the image
	Mat filtered_img = image;
    for (int i = 0; i < filters.size(); i++)
	{
        Filter* filter = filters[i];
		filtered_img = filter->filter(filtered_img);
	}

	// Edge Detection
	//Mat canny_output;
	//Canny(filtered_img, canny_output, thresh, thresh * 2, 3);
	Mat output;
	classifier.predict(image, output);
	//imshow("output", output);

	// Find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//, Point(0, 0));

	// Determine the Regions
	regions = vector<Region>(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		Region object;
		object.contour = contours[i];
		object.moment = moments(contours[i], false);    
		if (object.moment.m00 > 50)   // This is a wrapper at this point sometimes m00 comes out to be 0 and we later divide by 0 then
	    {                             // Therefore it is a preventative
		    object.center = Point2f(static_cast<float>(object.moment.m10 / object.moment.m00), 
		    							static_cast<float>(object.moment.m01 / object.moment.m00));
		    regions.push_back(object);
	    }
	}
}

void RegionFinder::add_filter(Filter* filter)
{
	filters.push_back(filter);
}
