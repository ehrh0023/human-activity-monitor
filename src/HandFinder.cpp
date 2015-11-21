#include "HandFinder.h"
#include "Region.h"
using namespace cv;

std::vector<Region> HandFinder::find_hands(cv::Mat frame, const std::vector<Region>& regions)
{	
	std::vector<Region> detectedObj;
	Region Face, Hand1, Hand2, handCenterObj;
    //Mat drawFace = Mat::zeros(frame.size(), CV_8UC3);     // Temp matrix for displaying calculated center point
    Face = regions[0];
	for (int i = 0; i < regions.size(); i++)              // for each region
    {
        Region region = regions[i];
        // Find Face, hand1, and hand2 
        if (Face.moment.m00 < region.moment.m00)          // If The largest region (Face) is less than current region
        { 
           Hand2 = Hand1;                                 // Shift the largest region to the front
           Hand1 = Face;                                
           Face = region;                               
        }
        else if (Hand1.moment.m00 < region.moment.m00)    // if the Face is larger but the largest hand is less than the current region
        {
           Hand2 = Hand1;                                 // Shift this region in
           Hand1 = region;
        }
        else if (Hand2.moment.m00 < region.moment.m00)    // else if the region is larger than the second hand
        {
           Hand2 = region;                                // shift it in to the end
        }
        
        //if (Hand2.center.x < region.center.x)
        //{                
        //    Hand1 = Face;
        //    Face = Hand2;
        //    Hand2 = region;
        //}
        //else if(Face.center.x < region.center.x)
        //{
	 	//    Hand1 = Face;
	 	//    Face = region;
        //}
        //else if (Hand1.center.x < region.center.x)
        //{
	 	//    Hand1 = region;
        //}
                
    }
    //Face.draw(drawFace);
	//Hand1.draw(drawFace);
	//Hand2.draw(drawFace);
    //imshow("Face", drawFace);
	detectedObj.push_back(Hand1);
	detectedObj.push_back(Face);
	detectedObj.push_back(Hand2);
    return detectedObj;
}