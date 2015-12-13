#include <opencv2/opencv.hpp>
#include <cstdint>
#include "../src/GaussianBayesClassifier.h"

using namespace cv;
using namespace std;

RNG rng(12345);
int main(int, char**)
{
	GaussianBayesClassifier bayes;

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


		if (image.empty() || mask.empty())
			cout << i << endl;
		else
		bayes.train(image, mask, true);
	}

	bayes.save("BayesPreset");

	Mat frame;
	Mat frame_gray;
	Mat canny_output;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	VideoCapture cap("../assets/flap_blur.avi");
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	while (1)
	{

		if (!cap.read(frame)) // get a new frame from camera
			break;

		Mat output;
		bayes.predict(frame, output);

		imshow("orig", frame);
		imshow("output", output);

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