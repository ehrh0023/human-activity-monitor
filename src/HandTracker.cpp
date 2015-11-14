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
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open the web cam");
    }

	// Initialize the classifier
	classifier.load("../assets/BayesPresetXYZ");
}

HandTracker::HandTracker(std::string file_name) :
    cap(file_name),
    regionFinder(),
	first(true),
	stats()
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file");
    }

    // Initialize the classifier
	classifier.load("../assets/BayesPresetXYZ");
}

HandTracker::~HandTracker()
{
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

	Mat output;
	classifier.predict(frame, output);

	//morphological opening (removes small objects from the foreground)
	erode(output, output, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(output, output, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	dilate(output, output, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(output, output, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    // Need center point of face for later tracking of frequency


    // Find the Regions
	std::vector<Region> regions;
	regionFinder.find(output, regions);
	
    // Display Regions
	Mat drawing = Mat::zeros(output.size(), CV_8UC3);
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

	imshow("output", output);
    imshow("frame", frame);
    imshow("edges", drawing);

	stats.update(output, regions);

	return frame;
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
