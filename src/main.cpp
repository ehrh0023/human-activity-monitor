#include "HandTracker.h"
#include <iostream>
#include <stdexcept>

int main(int, char**)
{
	try
	{
		// "C:/Users/User/Desktop/flap_blur.avi"
		HandTracker app("../assets/flap_blur.avi");
		return app.run();
	}
	catch (std::exception* e)
	{
		std::cout << e->what();
	}

	return -1;
}


