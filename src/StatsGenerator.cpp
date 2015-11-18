#include "StatsGenerator.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>

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
    Point2f handCenter(0, 0);   // Averaged hand coordinate point
    double distx, disty, velocity = 0;
    
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
           cycle = false;     // reset cycles
           cycle2 = false;    // reset cycles
           cycFramesLat = cycFrames;   // Latch cycles frames  
           Cycles++;                   // Add a cycle count
           cout << "distance is: " << distance << endl; 
           cout << "Frames: " << cycFrames << endl;
           velocity = distance / cycFramesLat;      // This gives distance per second or pixels per second
           cout << "Velocity is: " << velocity << endl;
           
           
           distance = 0;   // reset distance
		   cycFrames = 0;  // Reset cycles frames for next cycle
           //cout << Cycles << endl;
           
        }    
        cycle = true;   // Started the cycle
    }
    else
    {
       cycle2 = true;   // hands are below the face center point
       cycFrames++;
       distx = abs(handCenter.x - handCenterLast.x);
	   disty = abs(handCenter.y - handCenterLast.y);
       distance += ((distx + disty) / 2);                   // Average x and y distances for a single distance
    } 
    handCenterLast = handCenter;
    frames++;
    /*************************************************************************************
    ******** Output Metrics **************************************************************
    *************************************************************************************/

    // save as completed cycle towards frequency
    outdata.open(file_path, ios::app);                      //open the file to write to    
	if (!outdata.is_open())  // if not success, throw exception
	{
		throw new std::runtime_error("Cannot open file: " + file_path);
	}
    outdata << velocity << endl;
 	outdata.close();   // Close CSV file
 	return detectedObj;

    // Ship data to GUI

}