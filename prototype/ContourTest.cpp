/**
 * @function moments_demo.cpp
 * @brief Demo code to calculate moments
 * @author OpenCV team
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int thresh = 80;
const int max_thresh = 255;
RNG rng(12345);
int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	namedWindow("edges",1);
	while (1)
	{
		Mat frame;
		Mat frame_gray;
		Mat canny_output;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		if (!cap.read(frame)) // get a new frame from camera
			break;

		resize(frame, frame, Size(360, 240));
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		//blur(frame_gray, frame_gray, Size(3, 3));
		GaussianBlur(frame_gray, frame_gray, Size(15, 15), 0);

		Canny(frame_gray, canny_output, thresh, thresh * 2, 3);
		/// Find contours
		findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Get the moments
		vector<Moments> mu(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		///  Get the mass centers:
		vector<Point2f> mc(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
		}

		/// Draw contours
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 4, color, -1, 8, 0);
		}


		for (size_t i = 0; i < contours.size(); i++)
		{
			Rect r = boundingRect(contours[i]);
			rectangle(frame, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2);
		}

		imshow("frame", frame);
		imshow("edges", drawing);
		if(waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}