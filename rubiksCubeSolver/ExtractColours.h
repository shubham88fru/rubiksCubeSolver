#pragma once

#include <iostream>

//OpenCV includes
#include <opencv\cv.hpp>
#include <opencv2\highgui\highgui.hpp>

//the default frame width and height
#define frameWidth 360
#define frameHeigth 480

class ExtractColours
{
public:

	char Colours[54];						//store the initials of the colours detected.

	ExtractColours();
	//~ExtractColours();    //dont need the destructor for the time being!

private:

	//Mat object to grab frames
	cv::Mat _frame;

	//9 small cubes per face.
	cv::Rect _smallCubes[3][3];

	//the positions of the small cubes
	int _initialX = frameWidth / 4;
	int _initialY = frameHeigth / 6;
	int _gap = 30;				//gap between the small cubes.

	void _drawROIs();								//draw 9 sqaures for 9  cubes on a face
	void _getSmallCubeHSV(cv::Mat &Snap);			//extract the H S V values of small cubes of the face being shown.
	void _displaySmallCubeColour(cv::Mat &Snap);	//extract the colours of the small cubes
	void _loadSmallCubeColour(/*cv::Mat &Snap*/);		//Load the correct colour values into colour array.
};

