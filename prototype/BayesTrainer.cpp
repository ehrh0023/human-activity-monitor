#include <opencv2/opencv.hpp>

using namespace cv;
using namespace ml;

int main(int, char**)
{
	Ptr<NormalBayesClassifier> classifier = NormalBayesClassifier::create();

	for (int i = 1; i <= 11; ++i)
	{
		if (i == 3 || i == 4 || i == 7)
			continue;

		const std::string imgName = "C:/Users/brian_000/Desktop/Pictures/Unprocessed/pic";
		const std::string bwName = "C:/Users/brian_000/Desktop/Pictures/BW/picbw";
        Rect crop(10, 10, 1280,720);
		// read training image
		std::string normfile = imgName + std::to_string(i) + ".jpg";
		cv::Mat image = imread(normfile);
		
		//if (i == 7 || i == 8)
		//Mat image = cimage(crop);

		resize(image, image, Size(2000, 2000));

		cvtColor(image, image, CV_RGB2GRAY);
		image.convertTo(image, CV_32S); // train expects a matrix of integers
		image = image.reshape(0, image.rows*image.cols); // little trick number 2 convert your width x height, N channel image into a witdth*height row matrix by N columns, as each pixel should be considere as a training sample.

		// read mask image
		std::string maskfile = bwName + std::to_string(i) + ".jpg";
		cv::Mat mask = imread(maskfile);
		//if (i == 7 || i == 8)
		//Mat mask = cmask(crop);

		resize(mask, mask, Size(2000, 2000));

		cvtColor(mask, mask, CV_RGB2GRAY);
		mask.convertTo(mask, CV_32S); // train expects a matrix of integers
		mask = mask.reshape(0, image.rows*image.cols); // little trick number 2 convert your width x height, N channel image into a witdth*height row matrix by N columns, as each pixel should be considere as a training sample.

		if (i == 1)
		{
			Ptr<TrainData> td = TrainData::create(image, ROW_SAMPLE, mask);
			classifier->train(td);
		}
		else
		{
			std::cout << i;
			Ptr<TrainData> td = TrainData::create(image, ROW_SAMPLE, mask);
			classifier->train(td, NormalBayesClassifier::UPDATE_MODEL);
		}
	}
	classifier->save("Test.ymal");

	return 0;
}


