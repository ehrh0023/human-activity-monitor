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
    AlgStartTime = chrono::system_clock::now();
	ofstream outdata;                             //write variable
	outdata.open("Data.csv", ofstream::out);      // Clear CSV file on first frame
	if (!outdata.is_open())                       // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file: " + file_path);
    }
	outdata.close();
}

MovementSample StatsGenerator::create_sample(cv::Mat frame, HandInfo info, bool save)//cv::Point sample)
{
	/**************************************************************************************
	******** Object Tracking***************************************************************
	**************************************************************************************/
	// Metric Initializations
	Point2f handCenter(0, 0);   // Averaged hand center coordinate mid point
	double distx = 0;    
	double disty = 0;    
	double frequency = 0;
	double velocity = 0;
	handCenter = info.left_hand.center + info.right_hand.center;    // add hand center points for averaging.
	handCenter.y /= 2;   // average x val
	handCenter.x /= 2;   // average y val
	Region handCenterObj;                               
	handCenterObj.center = handCenter;              
	switch (State)
	{  
	case 0:
		if (info.head.center.y > handCenterObj.center.y)   // Hands are above center point of Head
		{
			State = 1;                                          // Wait for hands to go below center point of head
		}
		break;
	case 1:
		if (info.head.center.y < handCenterObj.center.y)   // Hands are below center point of head
		{
			start = chrono::system_clock::now();                // Start counting cycle time
			State = 2;                                          // Hands are below go capture data
		}
		break;
	case 2:
		distx = abs(handCenter.x - handCenterLast.x);           // Calculate x distance                                            
		disty = abs(handCenter.y - handCenterLast.y);           // Calculate y distance                                               
		distance += ((distx + disty) / 2);                      // Average x and y distances for a single distance
		if (info.head.center.y > handCenterObj.center.y)   // Hands went back above center point of head. End Cycle
		{                                                      
			State = 3;                                           // Go collect end data, end cycle, and reset for next cycle
		}
	break;
	case 3:
		end = chrono::system_clock::now();                      // Capture end time.
		cycleTime = end - start;                                // Cycle time in seconds to the nearest microsecond
		cycleOccurTime = end - AlgStartTime;                    // Calculate duration from start of algorithm to this cycle occured
		cycles++;                                               // Add a cycle count
		velocity = distance / cycleTime.count();                // Velocity (distance per second) or (pixels per second)
		frequency = cycles / cycleTime.count();                 // Frequency (cycles per second)
		distance = 0;                                           // Reset distance
		State = 1;
		break;
	}
	handCenterLast = handCenter;
	frames++;

	MovementSample sample;
	sample.time = cycleOccurTime.count();
	sample.velocity = velocity;
	sample.frequency = frequency;

	// Output Metrics 
	if(save)
	{
		save_sample(sample);
	}

	return sample;
}

void StatsGenerator::save_sample(MovementSample sample)
{
	ofstream outdata;   //write variable

	outdata.open(file_path, ofstream::app);                        //open the file to write to    
	if (!outdata.is_open())                                        // if not success, throw exception
	{
		throw new std::runtime_error("Cannot open file: " + file_path);
	}
	if ((sample.velocity > 0) & (sample.frequency > 0) & (sample.time > 0))
		outdata << sample.velocity << "," << sample.frequency << "," << sample.time << endl;   // output metrics as "velocity" "frequency" "Time since Algorithm start" in adjacent cells.
	outdata.close();   // Close CSV file    
}
