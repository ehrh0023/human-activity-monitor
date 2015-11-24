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
	stats(),
	loadedfromfile(false)
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open the web cam");
    }
}

HandTracker::HandTracker(std::string file_name) :
    cap(file_name),
    regionFinder(),
	stats(),
	loadedfromfile(true)
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file");
    }
}

void HandTracker::switch_source(int cam_id)
{
    cap = VideoCapture(cam_id);
	loadedfromfile = false;
}

void HandTracker::switch_source(std::string file_name)
{
    cap = VideoCapture(file_name);
	loadedfromfile = true;
}

MovementSample HandTracker::process(cv::Mat frame)
{	
    // Find the Regions
	std::vector<Region> regions;
	regionFinder.find(frame, regions);
    
	// Find the hands and Face
    HandInfo info;                        // Vector of detected objects                        
	info = HandFinder::find_hands(frame, regions);   // Find hands and Face and populate detected objects      
	// Display Regions
	//Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
    //Region region = info.left_hand;
    //region.draw(drawing);
    //region = info.right_hand;
    //region.draw(drawing);
    //region = info.head;
    //region.draw(drawing);
    //imshow("edges", drawing);
	                               
                                                           
	// Add a new sample
	MovementSample sample = stats.create_sample(frame, info, !stats.get_save_file().empty());

	// Send back that sample
	return sample;
}

int HandTracker::run()
{
    // Main loop
    while (1)
    {
		cv::Mat img = next_frame();

		if (!img.empty())
		{
			imshow("Camera", img);
			process(img);
		}

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

cv::Mat HandTracker::next_frame()
{
	cv::Mat frame;   // Use for each individual frame
	if (!cap.read(frame)) // read a new frame from video
	{
		//if not success, break loop
		cout << "Cannot read a frame from video stream" << endl;
		return cv::Mat();
	}
	return frame;
}

bool HandTracker::is_file()
{
	return cap.isOpened() && loadedfromfile;
}

bool HandTracker::is_cam()
{
	return cap.isOpened() && !loadedfromfile;
}

void HandTracker::restart_video()
{
	if (is_file())
	{
		cap.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
	}
}

void HandTracker::set_stats_file(std::string filename)
{
	stats.set_save_file(filename);
}

std::string HandTracker::get_stats_file()
{
	return stats.get_save_file();
}

cv::Size HandTracker::capture_size()
{
	int w = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int h = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	return cv::Size(w, h);
}
