#include "HandTracker.h"
#include <iostream>
#include <stdexcept>

int main(int, char**)
{
	try
	{
		// "C:/Users/User/Desktop/flap_blur.avi"
		HandTracker app("C:/Users/brian_000/Documents/GitHub/human-activity-monitor/assets/flap_blur.avi");
		return app.run();
	}
	catch (std::exception* e)
	{
		std::cout << e->what();
	}

	return -1;
}


