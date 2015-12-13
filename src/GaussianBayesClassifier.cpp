#include "GaussianBayesClassifier.h"
#include <fstream>
#include <math.h>
#include "Stats.h"

using namespace cv;
using namespace std;

GaussianBayesClassifier::GaussianBayesClassifier()
{
	clear();
}

void GaussianBayesClassifier::clear()
{
	samples = 0;
	rg_m = 0;
	rr_m = 0;
	gg_m = 0;

	red_mean = 0;
	green_mean = 0; 
}

void GaussianBayesClassifier::train(cv::Mat const& image, cv::Mat const& mask, bool update /*= false*/)
{
	Mat img;
	image.convertTo(img, CV_8UC3);
	uint8_t* pixelPtr = (uint8_t*)img.data;

	Mat msk;
	mask.convertTo(msk, CV_8UC3);
	uint8_t* maskPtr = (uint8_t*)msk.data;

	int cn = img.channels();
	cv::Scalar_<uint8_t> bgrPixel;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			float B = pixelPtr[i*img.cols*cn + j*cn + 0]; // B
			float G = pixelPtr[i*img.cols*cn + j*cn + 1]; // G
			float R = pixelPtr[i*img.cols*cn + j*cn + 2]; // R

			if (maskPtr[i*img.cols*cn + j*cn + 0]    > 100
				&& maskPtr[i*img.cols*cn + j*cn + 1] > 100
				&& maskPtr[i*img.cols*cn + j*cn + 2] > 100
				&& (R != 0 || G != 0 || B != 0))
			{
				float red = R / (R + G + B);
				float green = G / (R + G + B);

				samples++;
				float delta1 = (red - red_mean) / samples;
				red_mean += delta1;
				float delta2 = (green - green_mean) / samples;
				green_mean += delta2;

				rg_m += (samples - 1) * delta1 * delta2 - rg_m / samples;
				rr_m += (samples - 1) * delta1 * delta1 - rr_m / samples;
				gg_m += (samples - 1) * delta2 * delta2 - gg_m / samples;
			}
		}
	}
}

void GaussianBayesClassifier::predict(cv::Mat const& image, cv::Mat& output)
{
	Mat img;
	image.convertTo(img, CV_8UC3);
	uint8_t* pixelPtr = (uint8_t*)img.data;

	output = Mat(image.size(), CV_8UC1);
	uint8_t* outPtr = (uint8_t*)output.data;

	int cn = img.channels();
	cv::Scalar_<uint8_t> bgrPixel;

	float covrg = samples / (samples - 1.0) * rg_m;
	float varr = samples / (samples - 1.0) * rr_m;
	float varg = samples / (samples - 1.0) * gg_m;
	
	cv::Point2f mu(red_mean, green_mean);
	Matx<float, 2, 2> sigma(varr, covrg, covrg, varg);
	cv::RotatedRect ellipse = Stats::getErrorEllipse(1.5, mu, sigma);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			float B = pixelPtr[i*img.cols*cn + j*cn + 0]; // B
			float G = pixelPtr[i*img.cols*cn + j*cn + 1]; // G
			float R = pixelPtr[i*img.cols*cn + j*cn + 2]; // R
			
			if (R == 0 && G == 0 && B == 0)
				continue;

			float red = R / (R + G + B);
			float green = G / (R + G + B);
			cv::Point2f color(red, green);

			if (Stats::errorEllipseContains(color, ellipse))
			{
				outPtr[i*image.cols + j + 0] = 255;
			}
			else
			{
				outPtr[i*image.cols + j + 0] = 0;
			}
		}
	}
}

void GaussianBayesClassifier::save(std::string filename)
{
	ofstream myfile;
	myfile.open(filename);

	if (!myfile.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}

	myfile << samples << " "
		<< red_mean << " "
		<< green_mean << " "
		<< rg_m << " "
		<< gg_m << " "
		<< rr_m << " ";


	myfile.close();
}

void GaussianBayesClassifier::load(std::string filename)
{
	ifstream  myfile;
	myfile.open(filename);

	if (!myfile.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}

	myfile >> samples 
		>> red_mean 
		>> green_mean 
		>> rg_m
		>> gg_m
		>> rr_m;

	myfile.close();
}

