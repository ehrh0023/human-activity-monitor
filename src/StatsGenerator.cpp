#include "StatsGenerator.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <chrono>
#include <ctime>

using namespace cv;
using namespace std;


StatsGenerator::StatsGenerator(std::string file_name) :
	frames(1),
	file_path(file_name)    
{
	ofstream outdata;                             //write variable
	outdata.open("Data.csv", ofstream::out);      // Clear CSV file on first frame
	if (!outdata.is_open())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file: " + file_path);
    }
	outdata.close();
}

std::vector<Region> StatsGenerator::add_sample(cv::Mat frame, std::vector<Region>& detectedObj)//cv::Point sample)
{
	ofstream outdata;									 //write variable

    /**************************************************************************************
    ******** Object Tracking***************************************************************
    **************************************************************************************/
    // Metric Initializations
    Point2f handCenter(0, 0);   // Averaged hand coordinate point
    double distx = 0;
    double disty = 0;
    double frequency = 0;
    double velocity = 0;
	handCenter = detectedObj[2].center + detectedObj[0].center;   // add coordinate regions
	handCenter.y /= 2;   // average x val
	handCenter.x /= 2;   // average y val
    Region handCenterObj;                               
    handCenterObj.center = handCenter;
    detectedObj.push_back(handCenterObj);   // push up one level for drawing
    
    switch (State)
    {  
        case 0:
           if (detectedObj[1].center.y > handCenterObj.center.y)   // Hands are above center point of Head
           {
               State = 1;
           }
           break;
        case 1:
           if (detectedObj[1].center.y < handCenterObj.center.y)
           {
               start = chrono::system_clock::now();         // Start counting cycle time
               State = 2;    
           }
           break;
        case 2:
           distx = abs(handCenter.x - handCenterLast.x);                                                      
           disty = abs(handCenter.y - handCenterLast.y);                                                         
           distance += ((distx + disty) / 2);               // Average x and y distances for a single distance
           if (detectedObj[1].center.y > handCenterObj.center.y)
           {
              State = 3;
           }
           break;
        case 3:
           end = chrono::system_clock::now();
           cycleTime = end - start;    // Cycle time in seconds to the nearest microsecond
           cycles++;                   // Add a cycle count
           velocity = distance / cycleTime.count();      // Velocity (distance per second) or (pixels per second)
           frequency = cycles / cycleTime.count();       // Frequency (cycles per second)
           distance = 0;               // reset distance
           State = 1;
           break;
    }
    handCenterLast = handCenter;
    frames++;
    
    //frequency = cycles / 
    /*************************************************************************************
    ******** Output Metrics **************************************************************
    *************************************************************************************/
    outdata.open(file_path, ofstream::app);                      //open the file to write to    
	if (!outdata.is_open())  // if not success, throw exception
	{
		throw new std::runtime_error("Cannot open file: " + file_path);
	}
	outdata << velocity << "," << frequency << endl;       // output metrics as "velocity" "frequency" in adjacent cells.
	//cout << velocity << endl << frequency << endl;
 	outdata.close();   // Close CSV file
 	return detectedObj;

    // Ship data to GUI

}