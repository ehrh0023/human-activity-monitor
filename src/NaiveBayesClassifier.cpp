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
	pixelcount = 0;
	skincount = 0;
	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			colorCount[i][j] = 0;
			maskedCount[i][j] = 0;
		}
	}
}

void NaiveBayesClassifier::train(cv::Mat& image, cv::Mat& mask, bool update)
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
			colorCount[x][z]++;
			pixelcount++;

			if (maskPtr[i*imageXYZ.cols*cn + j*cn + 0]    > 100
				&& maskPtr[i*imageXYZ.cols*cn + j*cn + 1] > 100
				&& maskPtr[i*imageXYZ.cols*cn + j*cn + 2] > 100)
			{
				maskedCount[x][z]++;
				skincount++;
			}
		}
	}
}

cv::Mat* NaiveBayesClassifier::predict(cv::Mat& image)
{
	Mat imageXYZ;
	image.convertTo(imageXYZ, CV_8UC3);
	cvtColor(imageXYZ, imageXYZ, CV_RGB2XYZ);
	uint8_t* pixelPtr = (uint8_t*)imageXYZ.data;

	Mat* output = new Mat(imageXYZ.size(), CV_8UC1);
	uint8_t* outPtr = (uint8_t*)output->data;

	int cn = imageXYZ.channels();

	float skin = skincount / (float)pixelcount;
	float nskin = (1 - skin);

	for (int i = 0; i < imageXYZ.rows; i++)
	{
		for (int j = 0; j < imageXYZ.cols; j++)
		{
			uint8_t x = pixelPtr[i*imageXYZ.cols*cn + j*cn + 0]; // x
			uint8_t y = pixelPtr[i*imageXYZ.cols*cn + j*cn + 1]; // y
			uint8_t z = pixelPtr[i*imageXYZ.cols*cn + j*cn + 2]; // z

			float cskin = maskedCount[x][z] / (float)skincount;
			float skinc = cskin * skin;
			float cnskin = (colorCount[x][z] - maskedCount[x][z]) / (float)(pixelcount - skincount);
			float skinnc = cnskin * nskin;

			if (skinc > skinnc)
			{
				outPtr[i*imageXYZ.cols + j + 0] = 255; // x
			}
			else
			{
				outPtr[i*imageXYZ.cols + j + 0] = 0; // x
			}
		}
	}
	return output;
}

void NaiveBayesClassifier::save(std::string filename)
{
	ofstream myfile;
	myfile.open(filename);

	if (!myfile.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}

	myfile << pixelcount << ' ' << skincount << std::endl;

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile << colorCount[i][j] << ' ';
		}

		myfile << std::endl;
	}

	myfile << std::endl;

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile << maskedCount[i][j] << ' ';
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

	myfile >> pixelcount >> skincount;

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile >> colorCount[i][j];
		}
	}

	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			myfile >> maskedCount[i][j];
		}
	}

	myfile.close();
}
