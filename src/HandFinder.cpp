#include "HandFinder.h"
using namespace cv;

Point HandFinder::find_hands(const std::vector<Region>& regions)
{
	Point2f handCenter(0, 0);                             // Averaged hand coordinate point
	double distx = 0;
	double disty = 0;
	double velocity = 0;
	for (int i = 0; i < regions.size(); i++)              // for each region add the y vals and x vals
	{
		Region region = regions[i];
		handCenter += region.center;                      // add coordinate regions
	}
	handCenter.y /= ((regions.size()) / 2 + 1);           // This gives accurate x and y vals for the calculated center points
	handCenter.x /= ((regions.size()) / 2 + 1);           // This is a wrapper at this point

	return handCenter;
}