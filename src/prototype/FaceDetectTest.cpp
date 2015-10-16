#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int, char**)
{
	// Open the cascade
	CascadeClassifier face_cascade("C:/Users/User/Desktop/haarcascade_frontalface_default.xml");

	VideoCapture cap(0); //capture the video from webcam
	if (!cap.isOpened())  // if not success, exit program
	{
		std::cout << "Cannot open the web cam" << endl;
		return -1;
	}

	while (1)
	{

		Mat img;

		if (!cap.read(img)) // read a new frame from video
		{
			//if not success, break loop
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat gray;
		cvtColor(img, gray, COLOR_BGR2GRAY);

		// Find the faces
		std::vector<Rect> faces;
		face_cascade.detectMultiScale(gray, faces, 1.3, 5);

		// Display the found faces
		for (Rect point : faces)
		{
			rectangle(img, point, Scalar(255, 0, 0), 2);
		}

		imshow("img", img);//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
}