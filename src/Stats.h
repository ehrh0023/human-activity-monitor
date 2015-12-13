#pragma once
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

namespace Stats
{
	float sum(std::vector<float> data)
	{
		if (data.size() == 0)
			return 0;

		float sum = 0;
		for (int i = 0; i < data.size(); i++)
		{
			sum += data[i];
		}
		return sum;
	}

	float naive_covariance(std::vector<float> data1, std::vector<float> data2)
	{
		int n = data1.size();
		float sum12 = 0;
		float sum1 = sum(data1);
		float sum2 = sum(data2);

		for (int i = 0; i < n; i++)
		{
			sum12 += data1[i]*data2[i];
		}
		return (sum12 - (sum1*sum2 / n)) / n;
	}

	float online_covariance(std::vector<float> data1, std::vector<float> data2)
	{
		float mean1 = 0;
		float mean2 = 0;
		float M12 = 0;
		int n = data1.size();

		for (int i = 0; i < n; i++)
		{
			float delta1 = (data1[i] - mean1) / (i + 1);
			mean1 += delta1;
			float delta2 = (data2[i] - mean2) / (i + 1);
			mean2 += delta2;
			M12 += i * delta1 * delta2 - M12 / (i + 1);
		}
		return n / (n - 1.0) * M12;
	}

	cv::Matx<float, 2, 2> sqrtm(cv::Matx<float, 2, 2> M)
	{
		float s = sqrtf((float) determinant(M));
		float t = sqrtf(M.val[0] + M.val[3] + 2 * s);

		cv::Matx<float, 2, 2> r;
		r.val[0] = (M.val[0] + s) / t;
		r.val[1] = M.val[1] / t;
		r.val[2] = M.val[2] / t;
		r.val[3] = (M.val[3] + s) / t;

		return r;
	}

	//From http://www.visiondummy.com/2014/04/draw-error-ellipse-representing-covariance-matrix/
	cv::RotatedRect getErrorEllipse(double chisquare_val, cv::Point2f mean, cv::Matx<float, 2, 2> covmat)
	{

		//Get the eigenvalues and eigenvectors
		cv::Mat eigenvalues, eigenvectors;
		cv::eigen(covmat, true, eigenvalues, eigenvectors);

		//Calculate the angle between the largest eigenvector and the x-axis
		float angle = atan2f(eigenvectors.at<float>(0, 1), eigenvectors.at<float>(0, 0));

		//Shift the angle to the [0, 2pi] interval instead of [-pi, pi]
		if (angle < 0)
			angle += 6.28318530718;

		//Conver to degrees instead of radians
		angle = 180 * angle / 3.14159265359;

		//Calculate the size of the minor and major axes
		double halfmajoraxissize = chisquare_val*sqrtf(eigenvalues.at<float>(0));
		double halfminoraxissize = chisquare_val*sqrtf(eigenvalues.at<float>(1));

		//Return the oriented ellipse
		//The -angle is used because OpenCV defines the angle clockwise instead of anti-clockwise
		return cv::RotatedRect(mean, cv::Size2f(halfmajoraxissize, halfminoraxissize), -angle);
	}

	bool errorEllipseContains(cv::Point2f point, cv::RotatedRect rect)
	{
		cv::Point2f delta = point - rect.center;
		float u = cos(rect.angle)*delta.x + sin(rect.angle)*delta.y;
		float v = -sin(rect.angle)*delta.x + cos(rect.angle)*delta.y;

		float ua = u / rect.size.width;
		float vb = v / rect.size.height;
		float d2 = ua*ua + vb*vb;
		return d2 <= 1;
	}
}