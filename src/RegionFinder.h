#pragma once
#include <vector>
#include "Filter.h"
#include "Region.h"

struct RegionFinder
{
	int thresh;   /** How well does it detect edges */
	static const int max_thresh;   /** Maximum amount of threshold */
	std::vector<Filter*> filters; /** The filters attached to this object */


	/**
	 * Constructor for the Region Finder
	 * @param threshold of the edge detection
	 */
	RegionFinder(int threshold = 100);

	/**
	 * Destructor for the Region Finder
	 * Warning: Deletes all attached filters
	 */
	~RegionFinder();

	/**
	 * Detect the regions in the image
	 * @param image to check
	 * @return the found regions
	 */
	std::vector<Region> find(cv::Mat image);


	/**
	* Add a filter to the Finder
	* @param the filter to add
	*/
	void addFilter(Filter* filter);
};
