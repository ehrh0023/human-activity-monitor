/**
 * @function moments_demo.cpp
 * @brief Demo code to calculate moments
 * @author OpenCV team
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include "Object.h"

using namespace cv;
using namespace std;

int thresh = 80;
const int max_thresh = 255;
RNG rng(12345);

vector<Object> objects;

int main(int, char**)
{
	VideoCapture cap("c:/Users/Dennis/Desktop/box.mp4"); // open the default camera
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
		vector<Object> newObjects(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			Moments mu = moments(contours[i], false);
			newObjects[i].moments = mu;
			Point mc = Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
			newObjects[i].center = mc;

			float area = 0.0f;
			const float k_inv3 = 1.0f / 3.0f;
			for (int j = 0; j < contours[i].size(); j++)
			{
				// Triangle vertices, third vertex implied as (0, 0)
				Point2f p1(contours[i][j]);
				int j2 = j + 1 < contours[i].size() ? j + 1 : 0;
				Point2f p2(contours[i][j2]);

				float D = p1.cross(p2);
				float triangleArea = 0.5f * D;

				area += triangleArea;
			}
			newObjects[i].mass = area;
		}

		for (int i = 0; i < objects.size(); i++)
		{
			for (int j = 0; j < newObjects.size(); j++)
			{
				if (objects[i].compare(newObjects[j]))
				{
					objects[i].center = newObjects[j].center;
					objects[i].mass = newObjects[j].mass;
					objects[i].moments = newObjects[j].moments;
					objects[i].contour = newObjects[j].contour;
					break;
				}
			}
		}

		/// Draw contours
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, newObjects[i].center, 4, newObjects[i].color, -1, 8, 0);
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