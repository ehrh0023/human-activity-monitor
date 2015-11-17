#include "StatsGenerator.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace std;


StatsGenerator::StatsGenerator(std::string file_name) :
	distance(0),
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
    Point2f handCenter(0, 0);                             // Averaged hand coordinate point
    double distx, disty, velocity = 0;
	for (int i = 0; i < detectedObj.size(); i++)              // for each region add the y vals and x vals
	{
    	if ((i == 0) || (i == 2))
    	{
		   Region region = detectedObj[i];
		   handCenter += region.center;                      // add coordinate regions
	    }
	}
	handCenter.y /= 2;           // This gives accurate x and y vals for the calculated center points
	handCenter.x /= 2;           // This is a wrapper at this point
	cout << handCenter << endl;
    Region handCenterObj;                                 // Display handCenter for troubleshooting
    handCenterObj.center = handCenter;
    detectedObj.push_back(handCenterObj);
    
    
    
	//distx = sample.x - handCenterObj.center.x;			  // Calculate distance in x coordinate
	//disty = sample.y - handCenterObj.center.y;			  // Calculate distance in y coordinate
    //
	//handCenterObj.center = Point2f(sample);				  // Set the center point for the object to be displayed
    //
    //// calc Velocity
    //distance += ((distx + disty) / 2);                    // Average x and y distances for a single distance
    //
	//outdata.open(file_path, ios::app);                    //open the file to write to
    //
	//if (!outdata.is_open())  // if not success, throw exception
	//{
	//	throw new std::runtime_error("Cannot open file: " + file_path);
	//}
    //
    //if (frames == 30)                                     // Want 30 frames to establish seconds
    //{
	//   velocity = distance / 30;                          // This gives distance per second or pixels per second
    //   cout << velocity << endl;
    //   frames = 0;                                        // reset frame count
    //   distance = 0;                                      // Average distance per 30 frames
    //   outdata << velocity << endl;                       //Write data to .CSV file
    //}
    //frames++;                                             // Track frames over time
    //cout << frames << endl;


    // Compare averaged center point to face center point
    // if handCenter > faceCenter
    // {
    // While(handCenterY < faceCenterY)   // waits for hand amplitude to rise above faceCenter to complete cycle
    // {
    //    store detected hand points as collection of paired save points, one element per each pair of points
    //    this way we know how many frames occured in this cycle.
    // }
    // }
    /*************************************************************************************
    ******** Output Metrics **************************************************************
    *************************************************************************************/
    // find lowest y amplitude over saved frames
    // calculate velocity
    // save as completed cycle towards frequency
    
 	outdata.close();   // Close CSV file
 	return detectedObj;

    // Ship data to GUI

}