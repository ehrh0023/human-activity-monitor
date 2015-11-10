#include "HandTracker.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace std;

HandTracker::HandTracker(int cam_id):
    cap(cam_id),
    regionFinder(),
    first(true),
    cp("../assets/haarcascade_frontalface_default.xml"),
    hsv(new HSVFilter()),
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open the web cam");
    }

    // Add the HSV filter
    regionFinder.add_filter(hsv);
}

HandTracker::HandTracker(std::string file_name) :
    cap(file_name),
    regionFinder(),
	first(true),
	cp("../assets/haarcascade_frontalface_default.xml"),
    hsv(new HSVFilter()),
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file");
    }

    // Add the HSV filter
    regionFinder.add_filter(hsv);
}

HandTracker::~HandTracker()
{
    delete hsv;
}

void HandTracker::switch_source(int cam_id)
{
    cap = VideoCapture(cam_id);
}

void HandTracker::switch_source(std::string file_name)
{
    cap = VideoCapture(file_name);
}

void HandTracker::update()
{
    Mat frame;   // Use for each individual frame
    if (!cap.read(frame)) // read a new frame from video
    {
        //if not success, break loop
        cout << "Cannot read a frame from video stream" << endl;
        return;
    }

    // Initialize HSV profile and data csv file
	if (first)
    {
        hsv->passband = cp.determine_colors(frame);
        first = false;
    }

    // Need center point of face for later tracking of frequency


    // Find the Regions
	std::vector<Region> regions = regionFinder.find(frame);
	
    // Display Regions
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
    for (int i = 0; i < regions.size(); i++)
    {
        Region region = regions[i];
        region.draw(drawing);
    }

	// Display region bounds
    for (Region region : regions)
    {
		auto reg = region.contour;
		if (!reg.empty())
		{
			Rect r = boundingRect(reg);
			rectangle(frame, r.tl(), r.br(), Scalar(0, 255, 0), 2);
		}
    }

    imshow("frame", frame);
    imshow("edges", drawing);

	stats.update(frame, regions);
}

int HandTracker::run()
{
    // Main loop
    while (1)
    {
        update();

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
