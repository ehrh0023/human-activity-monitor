#include "HandTracker.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "HandFinder.h"

using namespace cv;
using namespace std;

HandTracker::HandTracker(int cam_id):
    cap(cam_id),
    regionFinder(),
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open the web cam");
    }
}

HandTracker::HandTracker(std::string file_name) :
    cap(file_name),
    regionFinder(),
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file");
    }
}

void HandTracker::switch_source(int cam_id)
{
    cap = VideoCapture(cam_id);
}

void HandTracker::switch_source(std::string file_name)
{
    cap = VideoCapture(file_name);
}

cv::Mat HandTracker::update()
{
	cv::Mat frame;   // Use for each individual frame
    if (!cap.read(frame)) // read a new frame from video
    {
        //if not success, break loop
        cout << "Cannot read a frame from video stream" << endl;
        return cv::Mat();
    }
	
    // Find the Regions
	std::vector<Region> regions;
	regionFinder.find(frame, regions);
	
	// Display Regions
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
    for (int i = 0; i < regions.size(); i++)
    {
        Region region = regions[i];
        region.draw(drawing);
    }
	
    //imshow("Edges", drawing);
	// Find the hands and Face
	//cv::Point center = HandFinder::find_hands(regions);
	Mat drawFace = Mat::zeros(frame.size(), CV_8UC3);       // Temp matrix for displaying calculated center point
    std::vector<Region> detectedObj;                        // Vector of detected objects                        
	detectedObj = HandFinder::find_hands(frame, regions);   // Find hands and Face and populate detected objects 
	detectedObj[0].draw(drawFace);                          // Draw Face
	detectedObj[2].draw(drawFace);                                         
                                   // Display Face                                      
                                                           
	// Add a new sample
	detectedObj = stats.add_sample(frame, detectedObj);//, center);
	detectedObj[3].draw(drawFace);
    imshow("Hands", drawFace);
	return frame;
}

int HandTracker::run()
{
    // Main loop
    while (1)
    {
        cv::Mat img = update();
		//imshow("Camera", img);

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
