#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

int main()
{
	cv::Mat lefthand = imread("C:/Users/Xincr/Desktop/lefthand.jpg");

	if (!lefthand.data)
	{
		return 0;
	}

	cv::Mat binary;

	cv::Mat markers;

	cv::cvtColor(lefthand, binary, CV_BGR2GRAY);
	

	cv::Mat fg;
	cv::erode(binary, fg, Mat(), Point(-1, -1), 2);


	cv::Mat bg;
	cv::dilate(binary, bg, Mat(), Point(-1, 1), 3);
	cv::threshold(bg, bg, 1, 255, THRESH_BINARY_INV);
	
	Mat MARKERS(binary.size(), CV_8U, Scalar(0));
	markers = fg + bg;


	
	markers.convertTo(markers, CV_32S);
	watershed(lefthand, markers);
	
	
	markers.convertTo(markers, CV_8U);
	
	
	imshow("lefthand", markers);
	imwrite("C:/Users/Xincr/Documents/Visual Studio 2015/Projects/Human Activity Monitor/lefthand.jpg",markers);

	while (1)
	{
		if (waitKey(30) == 27)
			break;

	}
			

	return 0;
}

