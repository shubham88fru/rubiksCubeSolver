#include "ExtractColours.h"

#define WebCam 0               //Set to '0' for inbuilt Camera or to '1' for an external camera.

#define smallCubeLength 30     //the length of small cubes

//Global Variables
int avgH[3][3], avgS[3][3], avgV[3][3];	//average H,S,V values
int currentFaceNum = 0;					//the current face number.
bool LoadColour = false;					//if the current face colours are detected correctly then capture the next frame

ExtractColours::ExtractColours()
{

	//access the webcam
	cv::VideoCapture Cap;
	Cap.open(WebCam);
	
	//Perform error check
	if (!Cap.isOpened()) {
	
		std::cerr << "Couldn't access the webcam!!,make sure that you'r trying to access the right webcam" << std::endl;
		
		int temp;
		std::cout << "Enter any key to exit." << std::endl;
		std::cin >> temp;
		exit(1);

	}

	//Window to display the frames
	cv::namedWindow("ExtractColours", 1);

	while (1) {
		
		//grab the frames
		Cap >> _frame;
		_drawROIs();
		
		//TODO:Some better functionality in case of errors.
		//Get the colour of the cubes again if the detected colours are wrong
		if (cv::waitKey(15) == 's') {
			
			//get a snapshot of the frame
			cv::Mat Snap;
			cv::Mat Colour;
			Cap >> Snap;
			Colour = Snap.clone();

			_getSmallCubeHSV(Snap);				//the user pressed 'S' means he is ready to get the HSV values.
			_displaySmallCubeColour(Colour);

			if (cv::waitKey(0) == 'l') {		//the detected colours are correct and the user wants to load them
			
				currentFaceNum++;
				_loadSmallCubeColour();
			}
		}

		//once we scan all the 6 faces correctly we will exit out of the loop
		if ((currentFaceNum + 1) > 6) {
			
			break;
		}

		//indicate the face number on the frame
		std::stringstream ss;
		ss << (currentFaceNum+1);
		cv::putText(_frame, "Scan Face " + ss.str(), cv::Point(100, 470), 2, 2, cv::Scalar(125, 0, 125), 2, 8);

		cv::imshow("ExtractColours", _frame);
		cv::waitKey(10);
	}
}

void ExtractColours::_drawROIs() {
	

	for (int i = 0; i < 3; i++) {			//loop through rows.
		
		for (int j = 0; j < 3; j++) {		//loop through columns.
			
			cv::Point pt1 = cv::Point((i*(_gap + smallCubeLength) + _initialX), (j*(_gap + smallCubeLength) + _initialY));
			cv::Point pt2 = cv::Point((i*(_gap + smallCubeLength) + _initialX+smallCubeLength), (j*(_gap + smallCubeLength) + _initialY+smallCubeLength));
			_smallCubes[i][j] = cv::Rect(pt1, pt2);
			cv::rectangle(_frame, _smallCubes[i][j], cv::Scalar(0, 0, 0), 2, 8);
			//just to ensure the numbering of cubes
			/*std::stringstream ss;
			ss << i;
			ss << j;
			cv::putText(_frame, ss.str(), pt1, 1, 1.1, cv::Scalar(255, 255, 0), 1, 8);*/
		}
	}
}			

void ExtractColours::_getSmallCubeHSV(cv::Mat &Snap){

	//the HSV version of the captured frame
	cv::Mat HSVFrame;

	//convert the the colour scheme of the frame for better reception of colour
	cv::cvtColor(Snap, HSVFrame,CV_BGR2HSV);
	//cv::imshow("HSV",HSVFrame);

	//The H,S,V vectors
	std::vector<int> HVec, SVec, VVec;
	
	//average H,S,V values
	avgH[3][3] = { 0 }, avgS[3][3] = { 0 }, avgV[3][3] = { 0 };

	//Indicate the face number.
	//cv::putText(_frame, "Face1", cv::Point(50, 50), 1, 1.1, cv::Scalar(255, 0, 0), 1, 8);

	for (int i = 0; i < 3; i++) {
	
		for (int j = 0; j < 3; j++) {

			for (int k = 5; k < smallCubeLength-5; k++) {
			
				for (int l = 5; l < smallCubeLength-5; l++) {
				
					//H value for every pixel
					HVec.push_back(HSVFrame.at<cv::Vec3b>(_smallCubes[i][j].x+k, _smallCubes[i][j].y+l)[0]);
					//avgH[i][j] += HVec[l];			//sum all the H values

					//S Value for every pixel
					SVec.push_back(HSVFrame.at<cv::Vec3b>(_smallCubes[i][j].x + k, _smallCubes[i][j].y + l)[1]);
					//avgS[i][j] += SVec[l];

					//V value for evry pixel
					VVec.push_back(HSVFrame.at<cv::Vec3b>(_smallCubes[i][j].x + k, _smallCubes[i][j].y + l)[2]);
					//avgV[i][j] += VVec[l];
				}
			}
			cv::circle(_frame, cv::Point(_smallCubes[i][j].x, _smallCubes[i][j].y), 5, cv::Scalar(255, 0, 0), -1, 8);

			for (int p = 0; p < HVec.size(); p++) {
				avgH[i][j] += HVec[p];
				avgS[i][j] += SVec[p];
				avgV[i][j] += VVec[p];
			}
			//calculate the average values of the H,S,V for a particular small cube.
			avgH[i][j] /= HVec.size();		//avg H value
			avgS[i][j] /= HVec.size();		//avg S value
			avgV[i][j] /= HVec.size();		//avg V value

			HVec.clear();
			SVec.clear();
			VVec.clear();
			
			//std::cout << "Colour " << i << j << " H= " << avgH[i][j] << " S= " << avgS[i][j] << " V= " << avgV[i][j] << "\n";

		}
	}

}

void ExtractColours::_displaySmallCubeColour(cv::Mat &Colour) {

	for (int i = 0; i < 3; i++) {
	
		for (int j = 0; j < 3; j++) {

			//compare the H Values
			if (avgH[i][j] >= 0 && avgH[i][j] <= 18 ) {	//orange colour
				if (avgS[i][j] > 70) {
				
					cv::putText(Colour, "O", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}
				else 
					cv::putText(Colour, "W", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
			}

			else if (avgH[i][j] > 18 && avgH[i][j] <= 30 ) {	//yellow colour detected
				if (avgS[i][j] > 70) {
				
					cv::putText(Colour, "Y", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}
				else 
					cv::putText(Colour, "W", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
			}

			else if (avgH[i][j] > 30 ) {
			
				if (avgS[i][j] >= 0 && avgS[i][j] <= 100) {	//White
				
					cv::putText(Colour, "W", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}

				else if (avgH[i][j] >= 70 && avgH[i][j] <= 90) {	//Green

					if (avgS[i][j] >= 235) {
						cv::putText(Colour, "R", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
					}
					
					else
					cv::putText(Colour, "G", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}
				else if ((avgH[i][j]>40 && avgH[i][j]<70) || (avgH[i][j] > 90 && avgH[i][j] <= 97)) {
					cv::putText(Colour, "R", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}

				else if (avgH[i][j] > 97 && avgH[i][j] <= 102) {
					
					/*if (avgS[i][j] >= 235) {*/
					
						cv::putText(Colour, "B", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);

					/*}*/

					//else /*if (avgS[i][j] < 230)*/ {
					//	cv::putText(Colour, "R", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
					//}
				}
				else if (avgH[i][j] > 102) {//Red
					cv::putText(Colour, "R", cv::Point(_smallCubes[j][i].x + (smallCubeLength / 2), _smallCubes[j][i].y + (smallCubeLength / 2)), 1, 1.1, cv::Scalar(0, 0, 0), 2, 8);
				}
				
			}
		}
	}

	cv::imshow("Detected Colours", Colour);
}

void ExtractColours::_loadSmallCubeColour(/*cv::Mat &Colour*/) {

	int index = 1;
	int multiplier = currentFaceNum - 1;
	int netIndex;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			netIndex = (multiplier * 9) + index;

			//compare the H Values
			if (avgH[i][j] >= 0 && avgH[i][j] <= 18) {	//orange colour
				
				if (avgS[i][j] > 70) {
				
					Colours[netIndex] = 'O';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
				}

				else {
					Colours[netIndex] = 'W';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;

				}

			}

			else if (avgH[i][j] > 18 && avgH[i][j] <= 30) {	//yellow colour detected
				
				if (avgS[i][j] > 70) {
				
					Colours[netIndex] = 'Y';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
				}

				else {
				
					Colours[netIndex] = 'W';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
				}

			}

			else if (avgH[i][j] > 30) {

				if (avgS[i][j] >= 0 && avgS[i][j] <= 100) {	//White
					Colours[netIndex] = 'W';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
					
				}

				else if (avgH[i][j]>=70 && avgH[i][j] <= 90) {	//Green
					if (avgS[i][j] >= 235) {
						Colours[netIndex] = 'R';
						std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
						index++;
					}
					
					else {
					
						Colours[netIndex] = 'G';
						std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
						index++;
					}
					
				}
				else if ((avgH[i][j]>40 && avgH[i][j]<70)||(avgH[i][j] > 90 && avgH[i][j] <= 97)) {
				
					Colours[netIndex] = 'R';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
				}
				else if (avgH[i][j] > 97 && avgH[i][j] <= 102) {

					/*if (avgS[i][j] >= 235) {*/
						Colours[netIndex] = 'B';
						std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
						index++;
						

					/*}*/

					//else /*if (avgS[i][j] < 230)*/ {
					//	Colours[netIndex] = 'R';
					//	std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					//	index++;
					//}
				}
				else if (avgH[i][j] > 102) {//Red
					Colours[netIndex] = 'R';
					std::cout << "Colour " << netIndex << " = " << Colours[netIndex] << "\n";
					index++;
				}

			}
		}
	}

}


//ExtractColours::~ExtractColours()
//{
//}
