#include <opencv2/opencv.hpp>
#include <cstdint>

class NaiveBayesClassifier
{
	float colors[256];

	static const int array_size = 256;

	int colorCount[array_size][array_size];
	int maskedCount[array_size][array_size];
	long int pixelcount;
	long int skincount;

public:

	NaiveBayesClassifier();
	void clear();

	void train(cv::Mat& image, cv::Mat& mask, bool update = false);

	cv::Mat* predict(cv::Mat& image);

	void save(std::string filename);
	void load(std::string filename);
};