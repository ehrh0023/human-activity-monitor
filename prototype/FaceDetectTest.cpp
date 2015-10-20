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

		Mat gray, HSV;
		cvtColor(img, HSV, COLOR_BGR2HSV);
		cvtColor(img, gray, COLOR_BGR2GRAY);

		// Find the faces
		std::vector<Rect> faces;
		face_cascade.detectMultiScale(gray, faces, 1.3, 5);

		// Display the found faces
		for (Rect point : faces)
		{
			rectangle(img, point, Scalar(255, 0, 0), 2);

			int minY = point.height * 0.25 + point.y;
			int minX = point.width  * 0.25 + point.x;
			int maxY = point.height * 0.75 + point.y;
			int maxX = point.width  * 0.75 + point.x;

			unsigned char minH = 255, minS = 255, minV = 255;
			unsigned char maxH = 0,   maxS = 0,   maxV = 0;
			for (int y = minY; y < maxY; y++)
			{
				for (int x = minX; x < maxX; x++)
				{
					cv::Vec3b pixel = HSV.at<Vec3b>(y, x);

					// H = pixel[0]
					if (pixel[0] < minH)
						minH = pixel[0];
					if (pixel[0] > maxH)
						maxH = pixel[0];

					// S = pixel[1]
					if (pixel[1] < minS)
						minS = pixel[1];
					if (pixel[1] > maxS)
						maxS = pixel[1];

					// V = pixel[2]
					if (pixel[2] < minV)
						minV = pixel[2];
					if (pixel[2] > maxV)
						maxV = pixel[2];
				}
			}

			std::cout << "H: " << (int)minH << " - " << (int)maxH << std::endl
				<< "S: " << (int)minS << " - " << (int)maxS << std::endl
				<< "V: " << (int)minV << " - " << (int)maxV << std::endl;
		}

		imshow("img", img);//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
}