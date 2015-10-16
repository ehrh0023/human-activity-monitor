#include "Filter.h"
#include <iostream>

using namespace cv;

HSVFilter::HSVFilter():
	passband()
{
	namedWindow("HSV Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Create trackbars in "Control" window
	createTrackbar("LowH", "HSV Control", &passband.min_hue, 179); //Hue (0 - 179)
	createTrackbar("HighH", "HSV Control", &passband.max_hue, 179);

	createTrackbar("LowS", "HSV Control", &passband.min_sat, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "HSV Control", &passband.max_sat, 255);

	createTrackbar("LowV", "HSV Control", &passband.min_val, 255);//Value (0 - 255)
	createTrackbar("HighV", "HSV Control", &passband.max_val, 255);
}

HSVFilter::~HSVFilter()
{
	destroyWindow("HSV Control");
}

Mat HSVFilter::filter(Mat image)
{
	Mat imgHSV;

	cvtColor(image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(passband.min_hue, passband.min_sat, passband.min_val), Scalar(passband.max_hue, passband.max_sat, passband.max_val), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	return imgThresholded;
}