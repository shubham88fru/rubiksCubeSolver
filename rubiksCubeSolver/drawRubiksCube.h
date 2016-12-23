#pragma once

//includes
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>

class drawRubiksCube
{
public:
	
	//drawRubiksCube(/*int argc, char* argv[]*/);
	void drawCube(int argc, char* argv[],char* colourArray,int size);
	//~drawRubiksCube();

private:
	
	//int _getColourAtIndex(int index);		//get the scrambled cube colour at a particular position
};

