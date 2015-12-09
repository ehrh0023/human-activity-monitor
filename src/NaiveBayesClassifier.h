#include <opencv2/opencv.hpp>
#include <cstdint>

/**
 * @class NaiveBayesClassifier
 * @brief Detect CIE-XYZ color patterns in an image using Baysean statistics
 */
class NaiveBayesClassifier
{
	float colors[256];

	static const int array_size = 256;

	int nskin_count[array_size][array_size];
	int skin_count[array_size][array_size];

public:

	/**
	 * Default Constructor
	 */
	NaiveBayesClassifier();

	/**
	 * Clear the training data
	 */
	void clear();

	/**
	 * Train the classifier from an image and mask
	 * @param image containing object of interest
	 * @param mask showing object in white and other in black
	 * @param update the current model or restart
	 */
	void train(cv::Mat const& image, cv::Mat const& mask, bool update = false);

	/**
	* Train the classifier from an image and mask
	 * @param image to search
	 * @param output showing detected regions
	 */
	void predict(cv::Mat const& image, cv::Mat& output);

	/**
	 * Save the classifier into a file
	 * @param filename to save to
	 */
	void save(std::string filename);

	/**
	 * Load the classifier from a file
	 * @param filename to load from
	 */
	void load(std::string filename);
};