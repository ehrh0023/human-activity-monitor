#include "RegionFinder.h"

using namespace cv;
using namespace std;

const int RegionFinder::max_thresh = 255;

RegionFinder::RegionFinder(int threshold /*= 100*/) 
	:thresh(threshold)
{
	// Initialize the classifier
	classifier.load("../assets/BayesPreset");
}

RegionFinder::~RegionFinder()
{
	for (size_t i = 0; i < filters.size(); i++)
	{
		delete filters[i];
	}
}

void RegionFinder::find(Mat image, std::vector<Region>& regions)
{
	// Edge Detection
	Mat output;
	classifier.predict(image, output);

	//morphological opening (removes small objects from the foreground)
    erode(output, output, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dilate(output, output, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    
	//morphological closing (removes small holes from the foreground)
    dilate(output, output, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    erode(output, output, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	//imshow("output", output);

	// Find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//, Point(0, 0));

	// Determine the Regions
	regions = vector<Region>();
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
