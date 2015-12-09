#include "NaiveBayesClassifier.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

NaiveBayesClassifier::NaiveBayesClassifier()
{
	clear();
}

void NaiveBayesClassifier::clear()
{
	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			skin_count[i][j] = 0;
			nskin_count[i][j] = 0;
		}
	}
}

void NaiveBayesClassifier::train(cv::Mat const& image, cv::Mat const& mask, bool update)
{
	if (!update)
	{
		clear();
	}
	Mat imageXYZ;
	image.convertTo(imageXYZ, CV_8UC3);
	cvtColor(imageXYZ, imageXYZ, CV_RGB2XYZ);
	uint8_t* pixelPtr = (uint8_t*)imageXYZ.data;

	Mat maskXYZ;
	mask.convertTo(maskXYZ, CV_8UC3);
	uint8_t* maskPtr = (uint8_t*)maskXYZ.data;

	int cn = imageXYZ.channels();
	cv::Scalar_<uint8_t> bgrPixel;

	for (int i = 0; i < imageXYZ.rows; i++)
	{
		for (int j = 0; j < imageXYZ.cols; j++)
		{
			uint8_t x = pixelPtr[i*imageXYZ.cols*cn + j*cn + 0]; // x
			uint8_t y = pixelPtr[i*imageXYZ.cols*cn + j*cn + 1]; // y
			uint8_t z = pixelPtr[i*imageXYZ.cols*cn + j*cn + 2]; // x

			if (maskPtr[i*imageXYZ.cols*cn + j*cn + 0]    > 100
				&& maskPtr[i*imageXYZ.cols*cn + j*cn + 1] > 100
				&& maskPtr[i*imageXYZ.cols*cn + j*cn + 2] > 100)
			{
				skin_count[x][z]++;
			}
			else
			{
				nskin_count[x][z]++;
			}
		}
	}
}

void NaiveBayesClassifier::predict(cv::Mat const& image, cv::Mat& output)
{
	Mat imageXYZ;
	image.convertTo(imageXYZ, CV_8UC3);
	cvtColor(imageXYZ, imageXYZ, CV_RGB2XYZ);
	uint8_t* pixelPtr = (uint8_t*)imageXYZ.data;

	output = Mat(imageXYZ.size(), CV_8UC1);
	uint8_t* outPtr = (uint8_t*)output.data;

	int cn = imageXYZ.channels();

	for (int i = 0; i < imageXYZ.rows; i++)
	{
		for (int j = 0; j < imageXYZ.cols; j++)
		{
			uint8_t x = pixelPtr[i*imageXYZ.cols*cn + j*cn + 0]; // x
			uint8_t y = pixelPtr[i*imageXYZ.cols*cn + j*cn + 1]; // y
			uint8_t z = pixelPtr[i*imageXYZ.cols*cn + j*cn + 2]; // z

			if (skin_count[x][z] > nskin_count[x][z])
			{
				outPtr[i*imageXYZ.cols + j + 0] = 255; // x
			}
			else
			{
				outPtr[i*imageXYZ.cols + j + 0] = 0; // x
			}
		}
	}
}

void NaiveBayesClassifier::save(std::string filename)
{
	ofstream myfile;
	myfile.open(filename);

	if (!myfile.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}


	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile << skin_count[i][j] << ' ';
		}

		myfile << std::endl;
	}

	myfile << std::endl;

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile << nskin_count[i][j] << ' ';
		}

		myfile << std::endl;
	}

	myfile.close();
}

void NaiveBayesClassifier::load(std::string filename)
{
	ifstream  myfile;
	myfile.open(filename);

	if (!myfile.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile >> skin_count[i][j];
		}
	}

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile >> nskin_count[i][j];
		}
	}

	myfile.close();
}
