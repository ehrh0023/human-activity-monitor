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

    if (detectedObj[1].center.y > handCenterObj.center.y)         // If the hand mid point is above the center point of the face start cycle
    {   
        if (cycle2 == true)   // end cycle
        {
           end = chrono::system_clock::now();
           cycleTime = end - start;    // Cycle time in seconds to the nearest microsecond
           velocity = distance / cycleTime.count();      // Velocity (distance per second) or (pixels per second)
           frequency = cycles / cycleTime.count();       // Frequency (cycles per second)
           cycles++;                   // Add a cycle count
           distance = 0;               // reset distance
           cycle = false;              // reset cycle
           cycle2 = false;             // reset cycle2
        }
        else
        {    
           cycle = true;   // Started the cycle
           start = chrono::system_clock::now();         // Start counting cycle time
        }
    }
    else
    { 
       cycle2 = true;   // hands are below the face center point
       distx = abs(handCenter.x - handCenterLast.x);
	   disty = abs(handCenter.y - handCenterLast.y);
       distance += ((distx + disty) / 2);               // Average x and y distances for a single distance 
      
    } 
    handCenterLast = handCenter;
    frames++;
    
    //frequency = cycles / 
    /*************************************************************************************
    ******** Output Metrics **************************************************************
    *************************************************************************************/

    // save as completed cycle towards frequency
    outdata.open(file_path, ios::app);                      //open the file to write to    
	if (!outdata.is_open())  // if not success, throw exception
	{
		throw new std::runtime_error("Cannot open file: " + file_path);
	}
	outdata << velocity << "," << frequency << endl;
 	outdata.close();   // Close CSV file
 	return detectedObj;

    // Ship data to GUI

}