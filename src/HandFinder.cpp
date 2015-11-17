#include "HandFinder.h"
#include "Region.h"
using namespace cv;

std::vector<Region> HandFinder::find_hands(cv::Mat frame, const std::vector<Region>& regions)
{	
	std::vector<Region> detectedObj;
	Region Face, Hand1, Hand2, handCenterObj;
    Mat drawFace = Mat::zeros(frame.size(), CV_8UC3);     // Temp matrix for displaying calculated center point
	//etectedObj = vector<Region>(3);
	for (int i = 0; i < regions.size(); i++)              // for each region add the y vals and x vals
    {
        Region region = regions[i];
        // Find Face, hand1, and hand2 
        if (region.moment.m00 > Face.moment.m00)          // If current region is greater than current largest object Face then
        {
           Hand1 = Face;                                  // The second largest object is hand1 
           Face = region;                                 // The largest object is the Face
        }
        else
        {
           Hand2 = region;                                // The next largest object is hand2
        }
        //handCenter += Hand1.center;                     // add coordinate regions
    }
	detectedObj.push_back(Hand1);
	detectedObj.push_back(Face);
	detectedObj.push_back(Hand2);
    return detectedObj;
}