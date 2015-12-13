#include <opencv2/opencv.hpp>
#include <cstdint>

/**
* @class GaussianBayesClassifier
*/
class GaussianBayesClassifier
{
	long int samples;

	float red_mean;
	float green_mean;

	float rg_m;
	float gg_m;
	float rr_m;

public:

	/**
	* Default Constructor
	*/
	GaussianBayesClassifier();

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