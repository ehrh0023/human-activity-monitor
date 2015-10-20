#include "ColorProfiler.h"
using namespace cv;

ColorProfiler::ColorProfiler(std::string const& filename):
	face_cascade(filename)
{
	if (face_cascade.empty())
	{
		std::string err_msg = "Face Cascade " + filename + " could not be loaded";
		throw std::exception(err_msg.c_str());
	}
}

HSVRange ColorProfiler::determine_colors(Mat img)
{
	Mat gray, HSV;
	cvtColor(img, HSV, COLOR_BGR2HSV);
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// Find the faces
	std::vector<Rect> faces;
	face_cascade.detectMultiScale(gray, faces, 1.3, 5);
	
	// Determine center point of head

	// Display the found faces
	for (Rect point : faces)
	{
		rectangle(img, point, Scalar(255, 0, 0), 2);

		int minY = point.height * 0.25 + point.y;
		int minX = point.width  * 0.25 + point.x;
		int maxY = point.height * 0.75 + point.y;
		int maxX = point.width  * 0.75 + point.x;

		HSVRange range;

		range.min_hue = 255;
		range.min_sat = 255;
		range.min_val = 255;
		range.max_hue = 0;
		range.max_sat = 0;
		range.max_val = 0;
		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				cv::Vec3b pixel = HSV.at<Vec3b>(y, x);

				// H = pixel[0]
				if (pixel[0] < range.min_hue)
					range.min_hue = pixel[0];
				if (pixel[0] > range.max_hue)
					range.max_hue = pixel[0];

				// S = pixel[1]
				if (pixel[1] < range.min_sat)
					range.min_sat = pixel[1];
				if (pixel[1] > range.max_sat)
					range.max_sat = pixel[1];

				// V = pixel[2]
				if (pixel[2] < range.min_val)
					range.min_val = pixel[2];
				if (pixel[2] > range.max_val)
					range.max_val = pixel[2];
			}
		}

		return range;
	}

	return HSVRange();
}
