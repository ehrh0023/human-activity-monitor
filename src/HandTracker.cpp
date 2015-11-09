#include "HandTracker.h"

#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace std;


HandTracker::HandTracker(int cam_id):
    cap(cam_id),
    regionFinder(),
    first(true),
    cp("C:/Users/brian_000/Documents/GitHub/human-activity-monitor/assets/haarcascade_frontalface_default.xml"),
    hsv(new HSVFilter()),
    distance(0),
    frames(1)
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open the web cam");
    }

    // Add the HSV filter
    regionFinder.add_filter(hsv);
}

HandTracker::HandTracker(std::string file_name) :
    cap(file_name),
    regionFinder(),
	first(true),
	cp("C:/Users/brian_000/Documents/GitHub/human-activity-monitor/assets/haarcascade_frontalface_default.xml"),
    hsv(new HSVFilter()),
    distance(0)
    
{
    if (!cap.isOpened())  // if not success, throw exception
    {
        throw new std::runtime_error("Cannot open file");
    }

    // Add the HSV filter
    regionFinder.add_filter(hsv);
}

HandTracker::~HandTracker()
{
    delete hsv;
}

void HandTracker::switch_source(int cam_id)
{
    cap = VideoCapture(cam_id);
}

void HandTracker::switch_source(std::string file_name)
{
    cap = VideoCapture(file_name);
}

void HandTracker::update()
{
    Mat frame;   // Use for each individual frame
    if (!cap.read(frame)) // read a new frame from video
    {
        //if not success, break loop
        cout << "Cannot read a frame from video stream" << endl;
        return;
    }
    ofstream outdata;                             //write variable
    // Initialize HSV profile and data csv file
	if (first)
    {
        hsv->passband = cp.determine_colors(frame);
        outdata.open("Data.csv", ofstream::out);      // Clear CSV file on first frame
        outdata.close();
        first = false;
    }

    // Need center point of face for later tracking of frequency


    // Find the Regions
	std::vector<Region> regions = regionFinder.find(frame);

    // Display Regions
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3);

    for (int i = 0; i < regions.size(); i++)
    {
        Region region = regions[i];
        region.draw(drawing);
    }

	// Display region bounds
    for (Region region : regions)
    {
		auto reg = region.contour;
		if (!reg.empty())
		{
			Rect r = boundingRect(reg);
			rectangle(frame, r.tl(), r.br(), Scalar(0, 255, 0), 2);
		}
    }

    imshow("frame", frame);
    imshow("edges", drawing);


    /**************************************************************************************
    ******** Object Tracking***************************************************************
    **************************************************************************************/
    Point2f handCenter(0, 0);                             // Averaged hand coordinate point
    Mat drawCenter = Mat::zeros(frame.size(), CV_8UC3);   // Temp matrix for displaying calculated center point
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
    Region handCenterObj;                                 // Display handCenter for troubleshooting
    distx = handCenter.x - handCenterObj.center.x;        // Calculate distance in x coordinate
    disty = handCenter.y - handCenterObj.center.y;        // Calculate distance in y coordinate
    
    handCenterObj.center = Point2f(handCenter);           // Set the center point for the object to be displayed
    handCenterObj.draw(drawCenter);                       // Draw the center point
    imshow("Center", drawCenter);                         // Display the center point
    
    // calc Velocity
    distance += ((distx + disty) / 2);                    // Average x and y distances for a single distance

	outdata.open("Data.csv", ios::app);                   //open the file to write to
	
    if (frames == 30)                                     // Want 30 frames to establish seconds
    {
	   velocity = distance / 30;                          // This gives distance per second or pixels per second
       cout << velocity << endl;
       frames = 0;                                        // reset frame count
       distance = 0;                                      // Average distance per 30 frames
       outdata << velocity << endl;                       //Write data to .CSV file
    }
    frames++;                                             // Track frames over time
    cout << frames << endl;
    
    

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

    // Ship data to GUI

}

int HandTracker::run()
{
    // Main loop
    while (1)
    {
        update();

        //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }

    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
