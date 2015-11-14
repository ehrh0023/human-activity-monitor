#include <opencv2/opencv.hpp>
#include <cstdint>

/**
 * @class NaiveBayesClassifier
 * @brief Detect skin in an image using Baysean statistics
 */
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

	void train(cv::Mat const& image, cv::Mat const& mask, bool update = false);

	void predict(cv::Mat const& image, cv::Mat& output);

	void save(std::string filename);
	void load(std::string filename);
};