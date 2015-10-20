#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "RegionFinder.h"
#include "ColorProfiler.h"

using namespace cv;
using namespace std;

int main(int, char**)
{
	// Capture video from webcam or prerecorded file
	// "C:/Users/User/Desktop/flap_blur.avi"
	VideoCapture cap("C:/Users/brian_000/Desktop/flap_blur.avi"); //capture the video from webcam
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	// Matrix for image outputs 
	Mat frame;

	//Capture a temporary image from the camera for sizing
	Mat imgTmp;
	cap.read(imgTmp);
                                             
	// Create a region finder
	RegionFinder regionFinder;
	// Add an HSV filter
	HSVFilter* hsv = new HSVFilter();
	regionFinder.add_filter(hsv);

	ColorProfiler cp("C:/Users/brian_000/Documents/GitHub/human-activity-monitor/assets/haarcascade_frontalface_default.xml");

	bool first = true;

	// Main loop
	while (1)
	{
		Mat frame;   // Use for each individual frame
		if (!cap.read(frame)) // read a new frame from video
		{
			//if not success, break loop
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
/**************************************************************************************
******** Hue Extraction ***************************************************************
**************************************************************************************/
		if (first)
		{
			hsv->passband = cp.determine_colors(frame);
			first = false;
		}
		
		
		
        // Need center point of face for later tracking of frequency
        
        
/**************************************************************************************
******** Filter, Find, and Store Regions **********************************************
**************************************************************************************/
		std::vector<Region> regions = regionFinder.find(frame);

/**************************************************************************************
******** Display Regions **************************************************************
**************************************************************************************/
		Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
		for (Region region : regions)
		{
			region.draw(drawing);
		}		
		 
/**************************************************************************************
******** Display rectangles around bounded objects ************************************
**************************************************************************************/
		for (size_t i = 0; i < regions.size(); i++)
		{
			Rect r = boundingRect(regions[i].contour);
			rectangle(frame, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2);
		}

		imshow("frame", frame);
		imshow("edges", drawing);
		
		
/**************************************************************************************
******** Object Tracking***************************************************************
**************************************************************************************/
        Point2f handCenter(0,0);                              // Averaged hand coordinate point
        Mat drawCenter = Mat::zeros(frame.size(), CV_8UC3);   // Temp matrix for displaying calculated center point	
		for (Region region : regions)                         // for each region add the y vals and x vals
		{                                                     
		   handCenter += region.center;                       // add coordinate regions
		}                                                     
		handCenter = Point2f(static_cast<float>(handCenter.y/(regions.size())),   // Calculate the averages of each coordinate 
		                     static_cast<float>(handCenter.x/(regions.size())));     
		Region handCenterObj;                                 // Display handCenter for troubleshooting
		handCenterObj.center = Point2f(handCenter);           // Set the center point for the object to be displayed
		handCenterObj.draw(drawCenter);                       // Draw the center point
		imshow("Center", drawCenter);                         // Display the center point
		     
		
		// Compare averaged center point to face center point
		   // if handCenter > faceCenter
		   // {
		      // While(handCenterY < faceCenterY)   // waits for hand amplitude to rise above faceCenter to complete cycle
		      // {
		      //    store detected dhand points as collection of paired save points, one element per each pair of points
		      //    this way we know how many frames occured in this cycle. 
		      // }
	       // }
/*************************************************************************************
******** Output Metrics **************************************************************
*************************************************************************************/
		// find lowest y amplitude over saved frames
		// calculate velocity
		// save as completed cycle towards frequency
		// Ship data to GUI



		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}


