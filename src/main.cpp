#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ObjectFinder.h"

using namespace cv;
using namespace std;

int main(int, char**)
{
	// Capture video from webcam or prerecorded file
	// "C:/Users/User/Desktop/flap_blur.avi"
	VideoCapture cap(0); //capture the video from webcam
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	// Matrix for image outputs 
	Mat frame;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	// Create an object find
	ObjectFinder objectFinder;
	HSVFilter* hsv = new HSVFilter();
	objectFinder.filters.push_back(hsv);

	// Main loop
	while (1)
	{
		Mat frame;   // Use for each individual frame
		
		bool bSuccess = cap.read(frame);
		
		if (!cap.read(frame)) // read a new frame from video
		{
			//if not success, break loop
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		
		std::vector<Object> objects = objectFinder.find(frame);

		Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
		for (Object object : objects)
		{
			object.draw(drawing);
		}

		/*
		for (size_t i = 0; i < contours.size(); i++)
		{
			Rect r = boundingRect(contours[i]);
			rectangle(frame, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2);
		}*/

		imshow("frame", frame);
		imshow("edges", drawing);

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}


