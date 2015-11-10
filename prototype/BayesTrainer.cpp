#include <opencv2/opencv.hpp>
#include <cstdint>
#include "../src/NaiveBayesClassifier.h"

using namespace cv;
using namespace std;

RNG rng(12345);
int main(int, char**)
{
	NaiveBayesClassifier bayes;

	for (int i = 1; i <= 12; ++i)
	{
		const std::string imgName = "C:/Users/Dennis/Desktop/Pictures/Unprocessed/pic";
		const std::string bwName = "C:/Users/Dennis/Desktop/Pictures/BW/picbw";

		// read training image
		std::string normfile = imgName + std::to_string(i) + ".jpg";
		cv::Mat image = imread(normfile);

		// read mask image
		std::string maskfile = bwName + std::to_string(i) + ".jpg";
		cv::Mat mask = imread(maskfile);

		bayes.train(image, mask, true);
	}

	bayes.save("BayesPresetXYZ");

	/*cv::Mat image = imread("C:/Users/Dennis/Desktop/Pictures/Unprocessed/pic4.jpg");

	Mat* output = bayes.predict(image);

	Mat imgThresholded = *output;
	//morphological opening (removes small objects from the foreground)
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	//dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	imshow("orig", image);
	imshow("new", *output);


	while(1)
	{
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			break;
		}
	}
	*/

	Mat frame;
	Mat frame_gray;
	Mat canny_output;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	VideoCapture cap(0);//"../assets/flap_blur.avi");
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	while (1)
	{

		if (!cap.read(frame)) // get a new frame from camera
			break;

		Mat imgThresholded;
		bayes.predict(frame, imgThresholded);

		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		findContours(imgThresholded, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Draw contours
		Mat drawing = Mat::zeros(imgThresholded.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
		}

		imshow("orig", frame);
		imshow("edges", drawing);

		if (!cap.read(frame)) // get a new frame from camera
			break;

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			break;
		}

	}

	return 0;
}