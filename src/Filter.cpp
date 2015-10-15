#include "Filter.h"
#include <iostream>

using namespace cv;

HSVFilter::HSVFilter() :
	low_hue(0),
	high_hue(179),
	low_sat(52),
	high_sat(119),
	low_val(150),
	high_val(255)
{
	namedWindow("HSV Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	std::cout << low_hue;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "HSV Control", &low_hue, 179); //Hue (0 - 179)
	createTrackbar("HighH", "HSV Control", &high_hue, 179);

	createTrackbar("LowS", "HSV Control", &low_sat, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "HSV Control", &high_sat, 255);

	createTrackbar("LowV", "HSV Control", &low_val, 255);//Value (0 - 255)
	createTrackbar("HighV", "HSV Control", &high_val, 255);
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

	inRange(imgHSV, Scalar(low_hue, low_sat, low_val), Scalar(high_hue, high_sat, high_val), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	return imgThresholded;
}