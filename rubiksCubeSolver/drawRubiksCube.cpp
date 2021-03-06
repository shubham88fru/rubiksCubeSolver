#include "drawRubiksCube.h"

/////////////////////////////////////////////////////////////
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
char defaultMessage[] = "Rotation Speed:";
char *message = defaultMessage;

static float speed = 1.0;				//speed of rotation

char _scrambledCubeColours[54];			//store the colour of the scrambled cube.
int var = 0;							//variable that contnously calls the keyboard function
int index = 0;							//access the elements of the moves strings
std::string movesString;				//store the moves found by the algorithm

//the unscrabled cube.
static int top[3][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } },       //white
right[3][3] = { { 1,1,1 },{ 1,1,1 },{ 1,1,1 } },				//orange
front[3][3] = { { 2,2,2 },{ 2,2,2 },{ 2,2,2 } },				//blue
back[3][3] = { { 3,3,3 },{ 3,3,3 },{ 3,3,3 } },					//green
bottom[3][3] = { { 4,4,4 },{ 4,4,4 },{ 4,4,4 } },				//yellow
left[3][3] = { { 5,5,5 },{ 5,5,5 },{ 5,5,5 } },					//red
temp[3][3];

int solve[300];
int count = 0;
int solve1 = 0;
static int rotation = 0;
int rotationcomplete = 0;
static GLfloat theta = 0.0;
static GLint axis = 0;
static GLfloat p = 0.0, q = 0.0, r = 0.0;
static GLint inverse = 0;
static GLfloat angle = 0.0;
int beginx = 0, beginy = 0;
int moving = 0;
static int speedmetercolor[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
static int speedmetercount = -1;

//String that stores the singmaster notation of the cube
std::string singMasterString[67];
int singMasterIndex = 0;


//The vertices of the 27 small cubes comprsing the bigger one
GLfloat vertices[][3] = { { -1.0,-1.0,-1.0 },
{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },
{ -1.0,1.0,-1.0 }, //center
{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },
{ 1.0,1.0,1.0 },
{ -1.0,1.0,1.0 },

{ -1.0,-3.0,-1.0 },
{ 1.0,-3.0,-1.0 },
{ 1.0,-1.0,-1.0 },
{ -1.0,-1.0,-1.0 },  //bottom center
{ -1.0,-3.0,1.0 },
{ 1.0,-3.0,1.0 },
{ 1.0,-1.0,1.0 },
{ -1.0,-1.0,1.0 },

{ -3.0,-1.0,-1.0 },
{ -1.0,-1.0,-1.0 },
{ -1.0,1.0,-1.0 },
{ -3.0,1.0,-1.0 },  //left center
{ -3.0,-1.0,1.0 },
{ -1.0,-1.0,1.0 },
{ -1.0,1.0,1.0 },
{ -3.0,1.0,1.0 },

{ 1.0,-1.0,-1.0 },
{ 3.0,-1.0,-1.0 },
{ 3.0,1.0,-1.0 },
{ 1.0,1.0,-1.0 }, // right center
{ 1.0,-1.0,1.0 },
{ 3.0,-1.0,1.0 },
{ 3.0,1.0,1.0 },
{ 1.0,1.0,1.0 },



{ -1.0,1.0,-1.0 },
{ 1.0,1.0,-1.0 },
{ 1.0,3.0,-1.0 },
{ -1.0,3.0,-1.0 }, // top center
{ -1.0,1.0,1.0 },
{ 1.0,1.0,1.0 },
{ 1.0,3.0,1.0 },
{ -1.0,3.0,1.0 },

{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },
{ 1.0,1.0,1.0 },
{ -1.0,1.0,1.0 }, //front center
{ -1.0,-1.0,3.0 },
{ 1.0,-1.0,3.0 },
{ 1.0,1.0,3.0 },
{ -1.0,1.0,3.0 },

{ -1.0,-1.0,-3.0 },
{ 1.0,-1.0,-3.0 },
{ 1.0,1.0,-3.0 },
{ -1.0,1.0,-3.0 }, //back center
{ -1.0,-1.0,-1.0 },
{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },
{ -1.0,1.0,-1.0 },

{ -3.0,1.0,-1.0 },
{ -1.0,1.0,-1.0 },
{ -1.0,3.0,-1.0 },
{ -3.0,3.0,-1.0 }, // top left center
{ -3.0,1.0,1.0 },
{ -1.0,1.0,1.0 },
{ -1.0,3.0,1.0 },
{ -3.0,3.0,1.0 },

{ 1.0,1.0,-1.0 },
{ 3.0,1.0,-1.0 },
{ 3.0,3.0,-1.0 },
{ 1.0,3.0,-1.0 }, // top right  center
{ 1.0,1.0,1.0 },
{ 3.0,1.0,1.0 },
{ 3.0,3.0,1.0 },
{ 1.0,3.0,1.0 },

{ -1.0,1.0,1.0 },
{ 1.0,1.0,1.0 },
{ 1.0,3.0,1.0 },
{ -1.0,3.0,1.0 }, // top front center
{ -1.0,1.0,3.0 },
{ 1.0,1.0,3.0 },
{ 1.0,3.0,3.0 },
{ -1.0,3.0,3.0 },


{ -1.0,1.0,-3.0 },
{ 1.0,1.0,-3.0 },
{ 1.0,3.0,-3.0 },
{ -1.0,3.0,-3.0 }, // top back center
{ -1.0,1.0,-1.0 },
{ 1.0,1.0,-1.0 },
{ 1.0,3.0,-1.0 },
{ -1.0,3.0,-1.0 },



{ -3.0,-3.0,-1.0 },
{ -1.0,-3.0,-1.0 },
{ -1.0,-1.0,-1.0 },
{ -3.0,-1.0,-1.0 },  //bottom left center
{ -3.0,-3.0,1.0 },
{ -1.0,-3.0,1.0 },
{ -1.0,-1.0,1.0 },
{ -3.0,-1.0,1.0 },

{ 1.0,-3.0,-1.0 },
{ 3.0,-3.0,-1.0 },
{ 3.0,-1.0,-1.0 },
{ 1.0,-1.0,-1.0 },  //bottom  right center
{ 1.0,-3.0,1.0 },
{ 3.0,-3.0,1.0 },
{ 3.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },

{ -1.0,-3.0,1.0 },
{ 1.0,-3.0,1.0 },
{ 1.0,-1.0,1.0 },
{ -1.0,-1.0,1.0 },  //bottom front center
{ -1.0,-3.0,3.0 },
{ 1.0,-3.0,3.0 },
{ 1.0,-1.0,3.0 },
{ -1.0,-1.0,3.0 },



{ -1.0,-3.0,-3.0 },
{ 1.0,-3.0,-3.0 },
{ 1.0,-1.0,-3.0 },
{ -1.0,-1.0,-3.0 },  //bottom back center
{ -1.0,-3.0,-1.0 },
{ 1.0,-3.0,-1.0 },
{ 1.0,-1.0,-1.0 },
{ -1.0,-1.0,-1.0 },

{ -3.0,1.0,-3.0 },
{ -1.0,1.0,-3.0 },
{ -1.0,3.0,-3.0 },
{ -3.0,3.0,-3.0 }, // top left back
{ -3.0,1.0,-1.0 },
{ -1.0,1.0,-1.0 },
{ -1.0,3.0,-1.0 },
{ -3.0,3.0,-1.0 },

{ -3.0,1.0,1.0 },
{ -1.0,1.0,1.0 },
{ -1.0,3.0,1.0 },
{ -3.0,3.0,1.0 }, // top left front
{ -3.0,1.0,3.0 },
{ -1.0,1.0,3.0 },
{ -1.0,3.0,3.0 },
{ -3.0,3.0,3.0 },

{ 1.0,1.0,-3.0 },
{ 3.0,1.0,-3.0 },
{ 3.0,3.0,-3.0 },
{ 1.0,3.0,-3.0 }, // top right  back
{ 1.0,1.0,-1.0 },
{ 3.0,1.0,-1.0 },
{ 3.0,3.0,-1.0 },
{ 1.0,3.0,-1.0 },

{ 1.0,1.0,1.0 },
{ 3.0,1.0,1.0 },
{ 3.0,3.0,1.0 },
{ 1.0,3.0,1.0 }, // top right  front
{ 1.0,1.0,3.0 },
{ 3.0,1.0,3.0 },
{ 3.0,3.0,3.0 },
{ 1.0,3.0,3.0 },

{ -3.0,-1.0,-3.0 },
{ -1.0,-1.0,-3.0 },
{ -1.0,1.0,-3.0 },
{ -3.0,1.0,-3.0 },  //ceneter left back
{ -3.0,-1.0,-1.0 },
{ -1.0,-1.0,-1.0 },
{ -1.0,1.0,-1.0 },
{ -3.0,1.0,-1.0 },

{ -3.0,-1.0,1.0 },
{ -1.0,-1.0,1.0 },
{ -1.0,1.0,1.0 },
{ -3.0,1.0,1.0 },  //center left front
{ -3.0,-1.0,3.0 },
{ -1.0,-1.0,3.0 },
{ -1.0,1.0,3.0 },
{ -3.0,1.0,3.0 },

{ 1.0,-1.0,-3.0 },
{ 3.0,-1.0,-3.0 },
{ 3.0,1.0,-3.0 },
{ 1.0,1.0,-3.0 }, // center right back
{ 1.0,-1.0,-1.0 },
{ 3.0,-1.0,-1.0 },
{ 3.0,1.0,-1.0 },
{ 1.0,1.0,-1.0 },

{ 1.0,-1.0,1.0 },
{ 3.0,-1.0,1.0 },
{ 3.0,1.0,1.0 },
{ 1.0,1.0,1.0 }, // center right front
{ 1.0,-1.0,3.0 },
{ 3.0,-1.0,3.0 },
{ 3.0,1.0,3.0 },
{ 1.0,1.0,3.0 },

{ -3.0,-3.0,-3.0 },
{ -1.0,-3.0,-3.0 },
{ -1.0,-1.0,-3.0 },
{ -3.0,-1.0,-3.0 },  //bottom left back
{ -3.0,-3.0,-1.0 },
{ -1.0,-3.0,-1.0 },
{ -1.0,-1.0,-1.0 },
{ -3.0,-1.0,-1.0 },

{ -3.0,-3.0,1.0 },
{ -1.0,-3.0,1.0 },
{ -1.0,-1.0,1.0 },
{ -3.0,-1.0,1.0 },  //bottom left front
{ -3.0,-3.0,3.0 },
{ -1.0,-3.0,3.0 },
{ -1.0,-1.0,3.0 },
{ -3.0,-1.0,3.0 },

{ 1.0,-3.0,-3.0 },
{ 3.0,-3.0,-3.0 },
{ 3.0,-1.0,-3.0 },
{ 1.0,-1.0,-3.0 },  //bottom  right back
{ 1.0,-3.0,-1.0 },
{ 3.0,-3.0,-1.0 },
{ 3.0,-1.0,-1.0 },
{ 1.0,-1.0,-1.0 },


{ 1.0,-3.0,1.0 },
{ 3.0,-3.0,1.0 },
{ 3.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },  //bottom  right front
{ 1.0,-3.0,3.0 },
{ 3.0,-3.0,3.0 },
{ 3.0,-1.0,3.0 },
{ 1.0,-1.0,3.0 },

{ 0.0,7.0,0.0 },
{ 0.0,7.5,0.0 },
{ 0.5,7.5,0.0 }, //speed meter
{ 0.5,7.0,0.0 }




};




GLfloat color[][3] = { { 1.0,1.0,1.0 },		//white ..top
{ 1.0,0.5,0.0 },							//orange
{ 0.0,0.0,1.0 },							//blue
{ 0.0,1.0,0.0 },							//green
{ 1.0,1.0,0.0 },						    //yellow
{ 1.0,0.0,0.0 },							 //red
{ 0.1,0.1,0.1 },							//grey used to represent faces of cube without colour
{ .6,.5,.6 }								//speed meter colour
};

//////////////////////////////////////////////////////////
void _getFrontFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {
	
		for (int j = 0; j < 3; j++) {
		
			front[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getRightFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			right[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getLeftFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			left[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getBackFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			back[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getTopFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			top[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getBottomFaceColours(int startIndex) {

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			bottom[i][j] = _scrambledCubeColours[startIndex];
			startIndex++;
		}
	}
}

void _getSingmasterString(int colourInts) {

	switch (colourInts) {
	
	case 0:
		singMasterString[singMasterIndex] = "U";
		singMasterIndex++;
		break;

	case 1:
		singMasterString[singMasterIndex] = "R";
		singMasterIndex++;
		break;

	case 2:
		singMasterString[singMasterIndex] = "F";
		singMasterIndex++;
		break;

	case 3:
		singMasterString[singMasterIndex] = "B";
		singMasterIndex++;
		break;

	case 4:
		singMasterString[singMasterIndex] = "D";
		singMasterIndex++;
		break;

	case 5:
		singMasterString[singMasterIndex] = "L";
		singMasterIndex++;
		break;
	}
}



void _output(int x, int y, char* string) {

	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

void _polygon(int a, int b, int c, int d, int e) {

	glColor3f(0, 0, 0);
	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glVertex3fv(vertices[e]);
	glEnd();


	glColor3fv(color[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glVertex3fv(vertices[e]);
	glEnd();
}

void _colorcube1()
{
	_polygon(6, 0, 3, 2, 1);
	_polygon(6, 2, 3, 7, 6);
	_polygon(6, 0, 4, 7, 3);    // center piece
	_polygon(6, 1, 2, 6, 5);
	_polygon(6, 4, 5, 6, 7);
	_polygon(6, 0, 1, 5, 4);


}
void _colorcube2()
{
	_polygon(6, 8, 11, 10, 9);
	_polygon(6, 10, 11, 15, 14);
	_polygon(6, 8, 12, 15, 11);    // bottom center
	_polygon(6, 9, 10, 14, 13);
	_polygon(6, 12, 13, 14, 15);
	_polygon(bottom[1][1]/*_getColourAtIndex(50)*/, 8, 9, 13, 12);


}
void _colorcube3()
{

	_polygon(6, 16, 19, 18, 17);
	_polygon(6, 18, 19, 23, 22);
	_polygon(left[1][1]/*_getColourAtIndex(23)*/, 16, 20, 23, 19);    // left center
	_polygon(6, 17, 18, 22, 21);
	_polygon(6, 20, 21, 22, 23);
	_polygon(6, 16, 17, 21, 20);


}
void _colorcube4()
{
	_polygon(6, 24, 27, 26, 25);
	_polygon(6, 26, 27, 31, 30);
	_polygon(6, 24, 28, 31, 27);    // right center
	_polygon(right[1][1]/*_getColourAtIndex(14)*/, 25, 26, 30, 29);
	_polygon(6, 28, 29, 30, 31);
	_polygon(6, 24, 25, 29, 28);


}
void _colorcube5()
{
	_polygon(6, 32, 35, 34, 33);
	_polygon(top[1][1]/*_getColourAtIndex(41)*/, 34, 35, 39, 38);
	_polygon(6, 32, 36, 39, 35);    // top center
	_polygon(6, 33, 34, 38, 37);
	_polygon(6, 36, 37, 38, 39);
	_polygon(6, 32, 33, 37, 36);


}
void _colorcube6()
{
	_polygon(6, 40, 43, 42, 41);
	_polygon(6, 42, 43, 47, 46);
	_polygon(6, 40, 44, 47, 43);    // front center
	_polygon(6, 41, 42, 46, 45);
	_polygon(front[1][1]/*_getColourAtIndex(5)*/, 44, 45, 46, 47);
	_polygon(6, 40, 41, 45, 44);


}
void _colorcube7()
{
	_polygon(back[1][1]/*_getColourAtIndex(32)*/, 48, 51, 50, 49);
	_polygon(6, 50, 51, 55, 54);
	_polygon(6, 48, 52, 55, 51);    //back center
	_polygon(6, 49, 50, 54, 53);
	_polygon(6, 52, 53, 54, 55);
	_polygon(6, 48, 49, 53, 52);


}
void _colorcube8()
{
	_polygon(6, 56, 59, 58, 57);
	_polygon(top[1][0]/*_getColourAtIndex(40)*/, 58, 59, 63, 62);
	_polygon(left[0][1]/*_getColourAtIndex(20)*/, 56, 60, 63, 59);    // top left center
	_polygon(6, 57, 58, 62, 61);
	_polygon(6, 60, 61, 62, 63);
	_polygon(6, 56, 57, 61, 60);


}
void _colorcube9()
{
	_polygon(6, 64, 67, 66, 65);
	_polygon(top[1][2]/*_getColourAtIndex(42)*/, 66, 67, 71, 70);
	_polygon(6, 64, 68, 71, 67);    // top right center
	_polygon(right[0][1]/*_getColourAtIndex(11)*/, 65, 66, 70, 69);
	_polygon(6, 68, 69, 70, 71);
	_polygon(6, 64, 65, 69, 68);


}
void _colorcube10()
{
	_polygon(6, 72, 75, 74, 73);
	_polygon(top[2][1]/*_getColourAtIndex(44)*/, 74, 75, 79, 78);
	_polygon(6, 72, 76, 79, 75);    // top front center
	_polygon(6, 73, 74, 78, 77);
	_polygon(front[0][1]/*_getColourAtIndex(2)*/, 76, 77, 78, 79);
	_polygon(6, 72, 73, 77, 76);


}
void _colorcube11()
{
	_polygon(back[0][1]/*_getColourAtIndex(29)*/, 80, 83, 82, 81);
	_polygon(top[0][1]/*_getColourAtIndex(38)*/, 82, 83, 87, 86);
	_polygon(6, 80, 84, 87, 83);    // top back center
	_polygon(6, 81, 82, 86, 85);
	_polygon(6, 84, 85, 86, 87);
	_polygon(6, 80, 81, 85, 84);


}
void _colorcube12()
{
	_polygon(6, 80 + 8, 83 + 8, 82 + 8, 81 + 8);
	_polygon(6, 82 + 8, 83 + 8, 87 + 8, 86 + 8);
	_polygon(left[2][1]/*_getColourAtIndex(26)*/, 80 + 8, 84 + 8, 87 + 8, 83 + 8);    // bottom left center
	_polygon(6, 81 + 8, 82 + 8, 86 + 8, 85 + 8);
	_polygon(6, 84 + 8, 85 + 8, 86 + 8, 87 + 8);
	_polygon(bottom[1][0]/*_getColourAtIndex(49)*/, 80 + 8, 81 + 8, 85 + 8, 84 + 8);


}
void _colorcube13()
{
	_polygon(6, 80 + 16, 83 + 16, 82 + 16, 81 + 16);
	_polygon(6, 82 + 16, 83 + 16, 87 + 16, 86 + 16);
	_polygon(6, 80 + 16, 84 + 16, 87 + 16, 83 + 16);    // bottom right center
	_polygon(right[2][1]/*_getColourAtIndex(17)*/, 81 + 16, 82 + 16, 86 + 16, 85 + 16);
	_polygon(6, 84 + 16, 85 + 16, 86 + 16, 87 + 16);
	_polygon(bottom[1][2]/*_getColourAtIndex(51)*/, 80 + 16, 81 + 16, 85 + 16, 84 + 16);


}
void _colorcube14()
{
	_polygon(6, 80 + 24, 83 + 24, 82 + 24, 81 + 24);
	_polygon(6, 82 + 24, 83 + 24, 87 + 24, 86 + 24);
	_polygon(6, 80 + 24, 84 + 24, 87 + 24, 83 + 24);    // bottom front center
	_polygon(6, 81 + 24, 82 + 24, 86 + 24, 85 + 24);
	_polygon(front[2][1]/*_getColourAtIndex(8)*/, 84 + 24, 85 + 24, 86 + 24, 87 + 24);
	_polygon(bottom[0][1]/*_getColourAtIndex(47)*/, 80 + 24, 81 + 24, 85 + 24, 84 + 24);


}
void _colorcube15()
{
	_polygon(back[2][1]/*_getColourAtIndex(35)*/, 112, 115, 114, 113);
	_polygon(6, 114, 115, 119, 118);
	_polygon(6, 112, 116, 119, 115);    // bottom back center
	_polygon(6, 113, 114, 118, 117);
	_polygon(6, 116, 117, 118, 119);
	_polygon(bottom[2][1]/*_getColourAtIndex(53)*/, 112, 113, 117, 116);


}
void _colorcube16()
{
	_polygon(back[0][2]/*_getColourAtIndex(30)*/, 120, 123, 122, 121);
	_polygon(top[0][0]/*_getColourAtIndex(37)*/, 122, 123, 127, 126);
	_polygon(left[0][0]/*_getColourAtIndex(19)*/, 120, 124, 127, 123);    // top left back
	_polygon(6, 121, 122, 126, 125);
	_polygon(6, 124, 125, 126, 127);
	_polygon(6, 120, 121, 125, 124);


}
void _colorcube17()
{
	_polygon(6, 128, 131, 130, 129);
	_polygon(top[2][0]/*_getColourAtIndex(43)*/, 130, 131, 135, 134);
	_polygon(left[0][2]/*_getColourAtIndex(21)*/, 128, 132, 135, 131);    // top left front
	_polygon(6, 129, 130, 134, 133);
	_polygon(front[0][0]/*_getColourAtIndex(1)*/, 132, 133, 134, 135);
	_polygon(6, 128, 129, 133, 132);


}
void _colorcube18()
{
	_polygon(back[0][0]/*_getColourAtIndex(28)*/, 136, 139, 138, 137);
	_polygon(top[0][2]/*_getColourAtIndex(39)*/, 138, 139, 143, 142);
	_polygon(6, 136, 140, 143, 139);    // top right back
	_polygon(right[0][2]/*_getColourAtIndex(12)*/, 137, 138, 142, 141);
	_polygon(6, 140, 141, 142, 143);
	_polygon(6, 136, 137, 141, 140);


}
void _colorcube19()
{
	_polygon(6, 144, 147, 146, 145);
	_polygon(top[2][2]/*_getColourAtIndex(45)*/, 146, 147, 151, 150);
	_polygon(6, 144, 148, 151, 147);    // top right front
	_polygon(right[0][0]/*_getColourAtIndex(10)*/, 145, 146, 150, 149);
	_polygon(front[0][2]/*_getColourAtIndex(3)*/, 148, 149, 150, 151);
	_polygon(6, 144, 145, 149, 148);


}
void _colorcube20()
{
	_polygon(back[1][2]/*_getColourAtIndex(33)*/, 152, 155, 154, 153);
	_polygon(6, 154, 155, 159, 158);
	_polygon(left[1][0]/*_getColourAtIndex(22)*/, 152, 156, 159, 155);    //center left back
	_polygon(6, 153, 154, 158, 157);
	_polygon(6, 156, 157, 158, 159);
	_polygon(6, 152, 153, 157, 156);


}
void _colorcube21()
{
	_polygon(6, 160, 163, 162, 161);
	_polygon(6, 162, 163, 167, 166);
	_polygon(left[1][2]/*_getColourAtIndex(24)*/, 160, 164, 167, 163);    // center left front
	_polygon(6, 161, 162, 166, 165);
	_polygon(front[1][0]/*_getColourAtIndex(4)*/, 164, 165, 166, 167);
	_polygon(6, 160, 161, 165, 164);


}
void _colorcube22()
{
	_polygon(back[1][0]/*_getColourAtIndex(31)*/, 168, 171, 170, 169);
	_polygon(6, 170, 171, 175, 174);
	_polygon(6, 168, 172, 175, 171);    // center right back
	_polygon(right[1][2]/*_getColourAtIndex(15)*/, 169, 170, 174, 173);
	_polygon(6, 172, 173, 174, 175);
	_polygon(6, 168, 169, 173, 172);


}
void _colorcube23()
{
	_polygon(6, 176, 179, 178, 177);
	_polygon(6, 178, 179, 183, 182);
	_polygon(6, 176, 180, 183, 179);    //center right front
	_polygon(right[1][0]/*_getColourAtIndex(13)*/, 177, 178, 182, 181);
	_polygon(front[1][2]/*_getColourAtIndex(6)*/, 180, 181, 182, 183);
	_polygon(6, 176, 177, 181, 180);


}
void _colorcube24()
{
	_polygon(back[2][2]/*_getColourAtIndex(54)*/, 184, 187, 186, 185);
	_polygon(6, 186, 187, 191, 190);
	_polygon(left[2][0]/*_getColourAtIndex(25)*/, 184, 188, 191, 187);    // bottom left back
	_polygon(6, 185, 186, 190, 189);
	_polygon(6, 188, 189, 190, 191);
	_polygon(bottom[2][0]/*_getColourAtIndex(52)*/, 184, 185, 189, 188);


}
void _colorcube25()
{
	_polygon(6, 192, 195, 194, 193);
	_polygon(6, 194, 195, 199, 198);
	_polygon(left[2][2]/*_getColourAtIndex(27)*/, 192, 196, 199, 195);    // bottom left front
	_polygon(6, 193, 194, 198, 197);
	_polygon(front[2][0]/*_getColourAtIndex(7)*/, 196, 197, 198, 199);
	_polygon(bottom[0][0]/*_getColourAtIndex(46)*/, 192, 193, 197, 196);


}
void _colorcube26()
{
	_polygon(back[2][0]/*_getColourAtIndex(34)*/, 200, 203, 202, 201);
	_polygon(6, 202, 203, 207, 206);
	_polygon(6, 200, 204, 207, 203);    // bottom right back
	_polygon(right[2][2]/*_getColourAtIndex(18),*/, 201, 202, 206, 205);
	_polygon(6, 204, 205, 206, 207);
	_polygon(bottom[2][2]/*_getColourAtIndex(54)*/, 200, 201, 205, 204);


}
void _colorcube27()
{
	_polygon(6, 208, 211, 210, 209);
	_polygon(6, 210, 211, 215, 214);
	_polygon(6, 208, 212, 215, 211);    // bottom right front
	_polygon(right[2][0]/*_getColourAtIndex(16)*/, 209, 210, 214, 213);
	_polygon(front[2][2]/*_getColourAtIndex(9),*/, 212, 213, 214, 215);
	_polygon(bottom[0][2]/*_getColourAtIndex(48)*/, 208, 209, 213, 212);


}
void _speedmeter() {

	glColor3fv(color[7]);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 7.2, 0.0);
	glVertex3f(1.0, 7.0, 0.0);
	glVertex3f(1.0, 7.5, 0.0);
	glEnd();


	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	_polygon(speedmetercolor[0], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 0.0, 0.0);
	_polygon(speedmetercolor[1], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, 0.0, 0.0);
	_polygon(speedmetercolor[2], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, 0.0, 0.0);
	_polygon(speedmetercolor[3], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 0.0, 0.0);
	_polygon(speedmetercolor[4], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.5, 0.0, 0.0);
	_polygon(speedmetercolor[5], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, 0.0, 0.0);
	_polygon(speedmetercolor[6], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.5, 0.0, 0.0);
	_polygon(speedmetercolor[7], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, 0.0, 0.0);
	_polygon(speedmetercolor[8], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.5, 0.0, 0.0);
	_polygon(speedmetercolor[9], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.0, 0.0, 0.0);
	_polygon(speedmetercolor[10], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.5, 0.0, 0.0);
	_polygon(speedmetercolor[11], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0, 0.0, 0.0);
	_polygon(speedmetercolor[12], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.5, 0.0, 0.0);
	_polygon(speedmetercolor[13], 216, 217, 218, 219);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0, 0.0, 0.0);
	_polygon(speedmetercolor[14], 216, 217, 218, 219);
	glPopMatrix();

	glColor3fv(color[7]);
	glBegin(GL_POLYGON);
	glVertex3f(9.5, 7.2, 0.0);
	glVertex3f(8.5, 7.0, 0.0);
	glVertex3f(8.5, 7.5, 0.0);
	glEnd();
}

//
//void _display() {
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//
//	_speedmeter();
//
//	glColor3fv(color[0]);
//	_output(0, 8, message);
//
//	glPushMatrix();
//	glRotatef(25.0 + p, 1.0, 0.0, 0.0);
//	glRotatef(-30.0 + q, 0.0, 1.0, 0.0);
//	glRotatef(0.0 + r, 0.0, 0.0, 1.0);
//
//
//	if (rotation == 0)
//	{
//
//		_colorcube1();
//		_colorcube2();
//		_colorcube3();
//		_colorcube4();
//		_colorcube5();
//		_colorcube6();
//		_colorcube7();
//		_colorcube8();
//		_colorcube9();
//		_colorcube10();
//		_colorcube11();
//		_colorcube12();
//		_colorcube13();
//		_colorcube14();
//		_colorcube15();
//		_colorcube16();
//		_colorcube17();
//		_colorcube18();
//		_colorcube19();
//		_colorcube20();
//		_colorcube21();
//		_colorcube22();
//		_colorcube23();
//		_colorcube24();
//		_colorcube25();
//		_colorcube26();
//		_colorcube27();
//	}
//	if (rotation == 1)
//	{
//
//
//		_colorcube1();
//		_colorcube2();
//		_colorcube3();
//		_colorcube4();
//		_colorcube6();
//		_colorcube7();
//		_colorcube12();
//		_colorcube13();
//		_colorcube14();
//		_colorcube15();
//		_colorcube20();
//		_colorcube21();
//		_colorcube22();
//		_colorcube23();
//		_colorcube24();
//		_colorcube25();
//		_colorcube26();
//		_colorcube27();
//
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Top");
//			glPopMatrix();
//			glRotatef(-theta, 0.0, 1.0, 0.0);
//		}
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "TopInverted");
//			glPopMatrix();
//			glRotatef(theta, 0.0, 1.0, 0.0);
//		}
//		_colorcube5();
//		_colorcube8();
//		_colorcube9();
//		_colorcube10();
//		_colorcube11();
//		_colorcube16();
//		_colorcube17();
//		_colorcube18();
//		_colorcube19();
//
//	}
//
//	if (rotation == 2)
//	{
//		_colorcube1();
//		_colorcube2();
//		_colorcube3();
//		_colorcube5();
//		_colorcube6();
//		_colorcube7();
//		_colorcube8();
//		_colorcube10();
//		_colorcube11();
//		_colorcube12();
//		_colorcube14();
//		_colorcube15();
//		_colorcube16();
//		_colorcube17();
//		_colorcube20();
//		_colorcube21();
//		_colorcube24();
//		_colorcube25();
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Right");
//			glPopMatrix();
//			glRotatef(-theta, 1.0, 0.0, 0.0);
//		}
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "RightInverted");
//			glPopMatrix();
//			glRotatef(theta, 1.0, 0.0, 0.0);
//		}
//		_colorcube4();
//		_colorcube9();
//		_colorcube13();
//		_colorcube18();
//		_colorcube19();
//		_colorcube22();
//		_colorcube23();
//		_colorcube26();
//		_colorcube27();
//	}
//
//
//
//	if (rotation == 3)
//	{
//		_colorcube1();
//		_colorcube2();
//		_colorcube3();
//		_colorcube4();
//		_colorcube5();
//		_colorcube7();
//		_colorcube8();
//		_colorcube9();
//		_colorcube11();
//		_colorcube12();
//		_colorcube13();
//		_colorcube15();
//		_colorcube16();
//		_colorcube18();
//		_colorcube20();
//		_colorcube22();
//		_colorcube24();
//		_colorcube26();
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Front");
//			glPopMatrix();
//			glRotatef(-theta, 0.0, 0.0, 1.0);
//		}
//
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "FrontInverted");
//			glPopMatrix();
//			glRotatef(theta, 0.0, 0.0, 1.0);
//		}
//
//		_colorcube6();
//		_colorcube10();
//		_colorcube14();
//		_colorcube17();
//		_colorcube19();
//		_colorcube21();
//		_colorcube23();
//		_colorcube25();
//		_colorcube27();
//	}
//
//	if (rotation == 4)
//	{
//		_colorcube1();
//		_colorcube2();
//		_colorcube4();
//		_colorcube5();
//		_colorcube6();
//		_colorcube7();
//		_colorcube9();
//		_colorcube10();
//		_colorcube11();
//		_colorcube13();
//		_colorcube14();
//		_colorcube15();
//		_colorcube18();
//		_colorcube19();
//		_colorcube22();
//		_colorcube23();
//		_colorcube26();
//		_colorcube27();
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Left");
//			glPopMatrix();
//			glRotatef(theta, 1.0, 0.0, 0.0);
//		}
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "LeftInverted");
//			glPopMatrix();
//			glRotatef(-theta, 1.0, 0.0, 0.0);
//		}
//		_colorcube3();
//		_colorcube8();
//		_colorcube12();
//		_colorcube16();
//		_colorcube17();
//		_colorcube20();
//		_colorcube21();
//		_colorcube24();
//		_colorcube25();
//	}
//
//
//	if (rotation == 5)
//	{
//		_colorcube1();
//		_colorcube2();
//		_colorcube3();
//		_colorcube4();
//		_colorcube5();
//		_colorcube6();
//		_colorcube8();
//		_colorcube9();
//		_colorcube10();
//		_colorcube12();
//		_colorcube13();
//		_colorcube14();
//		_colorcube17();
//		_colorcube19();
//		_colorcube21();
//		_colorcube23();
//		_colorcube25();
//		_colorcube27();
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Back");
//			glPopMatrix();
//			glRotatef(theta, 0.0, 0.0, 1.0);
//
//		}
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "BackInverted");
//			glPopMatrix();
//			glRotatef(-theta, 0.0, 0.0, 1.0);
//		}
//		_colorcube7();
//		_colorcube11();
//		_colorcube15();
//		_colorcube16();
//		_colorcube18();
//		_colorcube20();
//		_colorcube22();
//		_colorcube24();
//		_colorcube26();
//	}
//
//	if (rotation == 6)
//	{
//		_colorcube1();
//		_colorcube3();
//		_colorcube4();
//		_colorcube5();
//		_colorcube6();
//		_colorcube7();
//		_colorcube8();
//		_colorcube9();
//		_colorcube10();
//		_colorcube11();
//		_colorcube16();
//		_colorcube17();
//		_colorcube18();
//		_colorcube19();
//		_colorcube20();
//		_colorcube21();
//		_colorcube22();
//		_colorcube23();
//
//		if (inverse == 0)
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "Bottom");
//			glPopMatrix();
//			glRotatef(theta, 0.0, 1.0, 0.0);
//		}
//		else
//		{
//			glPushMatrix();
//			glColor3fv(color[0]);
//			_output(-11, 6, "BottomInverted");
//			glPopMatrix();
//
//			glRotatef(-theta, 0.0, 1.0, 0.0);
//		}
//		_colorcube2();
//		_colorcube12();
//		_colorcube13();
//		_colorcube14();
//		_colorcube15();
//		_colorcube24();
//		_colorcube25();
//		_colorcube26();
//		_colorcube27();
//
//	}
//	glPopMatrix();
//
//
//	/*glPushMatrix();
//	glTranslatef(-.5,-4,0);
//	glScalef(speed/4.5,1.0,1.0);
//	glTranslatef(0.5,4,0);
//	polygon(5,216,217,218,219);
//
//	glPopMatrix();
//	*/
//
//	glFlush();
//	glutSwapBuffers();
//}//TODO:reomove this display function
void _display()

{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Sleep(2000);

	glLoadIdentity();

	/*speedmeter();

	glColor3fv(color[0]);
	output(0, 8, message);*/

	//glPushMatrix();

	glRotatef(25.0 + p/*50.0*/, 1.0, 0.0, 0.0);
	glRotatef(-30.0 + q/*-60.0*/, 0.0, 1.0, 0.0);
	//glRotatef(0.0 + r, 0.0, 0.0, 1.0);


	if (rotation == 0)
	{

		_colorcube1();
		_colorcube2();
		_colorcube3();
		_colorcube4();
		_colorcube5();
		_colorcube6();
		_colorcube7();
		_colorcube8();
		_colorcube9();
		_colorcube10();
		_colorcube11();
		_colorcube12();
		_colorcube13();
		_colorcube14();
		_colorcube15();
		_colorcube16();
		_colorcube17();
		_colorcube18();
		_colorcube19();
		_colorcube20();
		_colorcube21();
		_colorcube22();
		_colorcube23();
		_colorcube24();
		_colorcube25();
		_colorcube26();
		_colorcube27();
	}
	if (rotation == 1)
	{


		_colorcube1();
		_colorcube2();
		_colorcube3();
		_colorcube4();
		_colorcube6();
		_colorcube7();
		_colorcube12();
		_colorcube13();
		_colorcube14();
		_colorcube15();
		_colorcube20();
		_colorcube21();
		_colorcube22();
		_colorcube23();
		_colorcube24();
		_colorcube25();
		_colorcube26();
		_colorcube27();

		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Top");
			//glPopMatrix();
			glRotatef(-theta, 0.0, 1.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube5();
			_colorcube8();
			_colorcube9();
			_colorcube10();
			_colorcube11();
			_colorcube16();
			_colorcube17();
			_colorcube18();
			_colorcube19();
		}
		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "TopInverted");
			//glPopMatrix();
			glRotatef(theta, 0.0, 1.0, 0.0);
			if (var == 0) system("b.exe");

			_colorcube5();
			_colorcube8();
			_colorcube9();
			_colorcube10();
			_colorcube11();
			_colorcube16();
			_colorcube17();
			_colorcube18();
			_colorcube19();
		}
		//colorcube5();
		//colorcube8();
		//colorcube9();
		//colorcube10();
		//colorcube11();
		//colorcube16();
		//colorcube17();
		//colorcube18();
		//colorcube19();

	}

	if (rotation == 2)
	{
		_colorcube1();
		_colorcube2();
		_colorcube3();
		_colorcube5();
		_colorcube6();
		_colorcube7();
		_colorcube8();
		_colorcube10();
		_colorcube11();
		_colorcube12();
		_colorcube14();
		_colorcube15();
		_colorcube16();
		_colorcube17();
		_colorcube20();
		_colorcube21();
		_colorcube24();
		_colorcube25();
		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Right");
			//glPopMatrix();
			glRotatef(-theta, 1.0, 0.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube4();
			_colorcube9();
			_colorcube13();
			_colorcube18();
			_colorcube19();
			_colorcube22();
			_colorcube23();
			_colorcube26();
			_colorcube27();
		}
		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "RightInverted");
			//glPopMatrix();
			glRotatef(theta, 1.0, 0.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube4();
			_colorcube9();
			_colorcube13();
			_colorcube18();
			_colorcube19();
			_colorcube22();
			_colorcube23();
			_colorcube26();
			_colorcube27();
		}
		//colorcube4();
		//colorcube9();
		//colorcube13();
		//colorcube18();
		//colorcube19();
		//colorcube22();
		//colorcube23();
		//colorcube26();
		//colorcube27();
		//Sleep(1000);
	}



	if (rotation == 3)
	{
		_colorcube1();
		_colorcube2();
		_colorcube3();
		_colorcube4();
		_colorcube5();
		_colorcube7();
		_colorcube8();
		_colorcube9();
		_colorcube11();
		_colorcube12();
		_colorcube13();
		_colorcube15();
		_colorcube16();
		_colorcube18();
		_colorcube20();
		_colorcube22();
		_colorcube24();
		_colorcube26();
		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Front");
			//glPopMatrix();
			glRotatef(-theta, 0.0, 0.0, 1.0);
			if (var == 0) system("b.exe");
			_colorcube6();
			_colorcube10();
			_colorcube14();
			_colorcube17();
			_colorcube19();
			_colorcube21();
			_colorcube23();
			_colorcube25();
			_colorcube27();
		}

		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "FrontInverted");
			//glPopMatrix();
			glRotatef(theta, 0.0, 0.0, 1.0);
			if (var == 0) system("b.exe");
			_colorcube6();
			_colorcube10();
			_colorcube14();
			_colorcube17();
			_colorcube19();
			_colorcube21();
			_colorcube23();
			_colorcube25();
			_colorcube27();
		}

		//colorcube6();
		//colorcube10();
		//colorcube14();
		//colorcube17();
		//colorcube19();
		//colorcube21();
		//colorcube23();
		//colorcube25();
		//colorcube27();
	}

	if (rotation == 4)
	{
		_colorcube1();
		_colorcube2();
		_colorcube4();
		_colorcube5();
		_colorcube6();
		_colorcube7();
		_colorcube9();
		_colorcube10();
		_colorcube11();
		_colorcube13();
		_colorcube14();
		_colorcube15();
		_colorcube18();
		_colorcube19();
		_colorcube22();
		_colorcube23();
		_colorcube26();
		_colorcube27();
		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Left");
			//glPopMatrix();
			glRotatef(theta, 1.0, 0.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube3();
			_colorcube8();
			_colorcube12();
			_colorcube16();
			_colorcube17();
			_colorcube20();
			_colorcube21();
			_colorcube24();
			_colorcube25();
		}
		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "LeftInverted");
			//glPopMatrix();
			glRotatef(-theta, 1.0, 0.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube3();
			_colorcube8();
			_colorcube12();
			_colorcube16();
			_colorcube17();
			_colorcube20();
			_colorcube21();
			_colorcube24();
			_colorcube25();
		}
		//colorcube3();
		//colorcube8();
		//colorcube12();
		//colorcube16();
		//colorcube17();
		//colorcube20();
		//colorcube21();
		//colorcube24();
		//colorcube25();
	}


	if (rotation == 5)
	{
		_colorcube1();
		_colorcube2();
		_colorcube3();
		_colorcube4();
		_colorcube5();
		_colorcube6();
		_colorcube8();
		_colorcube9();
		_colorcube10();
		_colorcube12();
		_colorcube13();
		_colorcube14();
		_colorcube17();
		_colorcube19();
		_colorcube21();
		_colorcube23();
		_colorcube25();
		_colorcube27();
		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Back");
			//glPopMatrix();
			glRotatef(theta, 0.0, 0.0, 1.0);
			if (var == 0) system("b.exe");
			_colorcube7();
			_colorcube11();
			_colorcube15();
			_colorcube16();
			_colorcube18();
			_colorcube20();
			_colorcube22();
			_colorcube24();
			_colorcube26();

		}
		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "BackInverted");
			//glPopMatrix();
			glRotatef(-theta, 0.0, 0.0, 1.0);
			if (var == 0) system("b.exe");
			_colorcube7();
			_colorcube11();
			_colorcube15();
			_colorcube16();
			_colorcube18();
			_colorcube20();
			_colorcube22();
			_colorcube24();
			_colorcube26();
		}
		//colorcube7();
		//colorcube11();
		//colorcube15();
		//colorcube16();
		//colorcube18();
		//colorcube20();
		//colorcube22();
		//colorcube24();
		//colorcube26();
	}

	if (rotation == 6)
	{
		_colorcube1();
		_colorcube3();
		_colorcube4();
		_colorcube5();
		_colorcube6();
		_colorcube7();
		_colorcube8();
		_colorcube9();
		_colorcube10();
		_colorcube11();
		_colorcube16();
		_colorcube17();
		_colorcube18();
		_colorcube19();
		_colorcube20();
		_colorcube21();
		_colorcube22();
		_colorcube23();

		if (inverse == 0)
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "Bottom");
			//glPopMatrix();
			glRotatef(theta, 0.0, 1.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube2();
			_colorcube12();
			_colorcube13();
			_colorcube14();
			_colorcube15();
			_colorcube24();
			_colorcube25();
			_colorcube26();
			_colorcube27();
		}
		else
		{
			//glPushMatrix();
			glColor3fv(color[0]);
			_output(-11, 6, "BottomInverted");
			//glPopMatrix();

			glRotatef(-theta, 0.0, 1.0, 0.0);
			if (var == 0) system("b.exe");
			_colorcube2();
			_colorcube12();
			_colorcube13();
			_colorcube14();
			_colorcube15();
			_colorcube24();
			_colorcube25();
			_colorcube26();
			_colorcube27();
		}
		//colorcube2();
		//colorcube12();
		//colorcube13();
		//colorcube14();
		//colorcube15();
		//colorcube24();
		//colorcube25();
		//colorcube26();
		//colorcube27();

	}
	//glPopMatrix();


	/*glPushMatrix();
	glTranslatef(-.5,-4,0);
	glScalef(speed/4.5,1.0,1.0);
	glTranslatef(0.5,4,0);
	polygon(5,216,217,218,219);

	glPopMatrix();
	*/

	glFlush();
	glutSwapBuffers();

}

void _transpose(char a) {
	if (a == 'r')
	{
		int temp;
		temp = right[0][0];
		right[0][0] = right[2][0];
		right[2][0] = right[2][2];
		right[2][2] = right[0][2];
		right[0][2] = temp;
		temp = right[1][0];
		right[1][0] = right[2][1];
		right[2][1] = right[1][2];
		right[1][2] = right[0][1];
		right[0][1] = temp;
	}
	if (a == 't')
	{
		int temp;
		temp = top[0][0];
		top[0][0] = top[2][0];
		top[2][0] = top[2][2];
		top[2][2] = top[0][2];
		top[0][2] = temp;
		temp = top[1][0];
		top[1][0] = top[2][1];
		top[2][1] = top[1][2];
		top[1][2] = top[0][1];
		top[0][1] = temp;
	}
	if (a == 'f')
	{
		int temp;
		temp = front[0][0];
		front[0][0] = front[2][0];
		front[2][0] = front[2][2];
		front[2][2] = front[0][2];
		front[0][2] = temp;
		temp = front[1][0];
		front[1][0] = front[2][1];
		front[2][1] = front[1][2];
		front[1][2] = front[0][1];
		front[0][1] = temp;
	}
	if (a == 'l')
	{
		int temp;
		temp = left[0][0];
		left[0][0] = left[2][0];
		left[2][0] = left[2][2];
		left[2][2] = left[0][2];
		left[0][2] = temp;
		temp = left[1][0];
		left[1][0] = left[2][1];
		left[2][1] = left[1][2];
		left[1][2] = left[0][1];
		left[0][1] = temp;
	}
	if (a == 'k')
	{
		int temp;
		temp = back[0][0];
		back[0][0] = back[2][0];
		back[2][0] = back[2][2];
		back[2][2] = back[0][2];
		back[0][2] = temp;
		temp = back[1][0];
		back[1][0] = back[2][1];
		back[2][1] = back[1][2];
		back[1][2] = back[0][1];
		back[0][1] = temp;
	}

	if (a == 'b')
	{
		int temp;
		temp = bottom[0][0];
		bottom[0][0] = bottom[2][0];
		bottom[2][0] = bottom[2][2];
		bottom[2][2] = bottom[0][2];
		bottom[0][2] = temp;
		temp = bottom[1][0];
		bottom[1][0] = bottom[2][1];
		bottom[2][1] = bottom[1][2];
		bottom[1][2] = bottom[0][1];
		bottom[0][1] = temp;
	}

}

void _topc()
{
	_transpose('t');
	int temp1 = front[0][0];
	int temp2 = front[0][1];
	int temp3 = front[0][2];

	front[0][0] = right[0][0];
	front[0][1] = right[0][1];
	front[0][2] = right[0][2];

	right[0][0] = back[0][0];
	right[0][1] = back[0][1];
	right[0][2] = back[0][2];

	back[0][0] = left[0][0];
	back[0][1] = left[0][1];
	back[0][2] = left[0][2];

	left[0][0] = temp1;
	left[0][1] = temp2;
	left[0][2] = temp3;

}
void _frontc()
{
	_transpose('f');
	int temp1 = left[0][2];
	int temp2 = left[1][2];
	int temp3 = left[2][2];

	left[0][2] = bottom[0][0];
	left[1][2] = bottom[0][1];
	left[2][2] = bottom[0][2];

	bottom[0][0] = right[2][0];
	bottom[0][1] = right[1][0];
	bottom[0][2] = right[0][0];

	right[2][0] = top[2][2];
	right[1][0] = top[2][1];
	right[0][0] = top[2][0];

	top[2][2] = temp1;
	top[2][1] = temp2;
	top[2][0] = temp3;
}
void _rightc()
{
	_transpose('r');
	int temp1 = top[0][2];
	int temp2 = top[1][2];
	int temp3 = top[2][2];

	top[0][2] = front[0][2];
	top[1][2] = front[1][2];
	top[2][2] = front[2][2];

	front[0][2] = bottom[0][2];
	front[1][2] = bottom[1][2];
	front[2][2] = bottom[2][2];


	bottom[0][2] = back[2][0];
	bottom[1][2] = back[1][0];
	bottom[2][2] = back[0][0];

	back[2][0] = temp1;
	back[1][0] = temp2;
	back[0][0] = temp3;

}
void _leftc()
{
	_transpose('l');
	int temp1 = front[0][0];
	int temp2 = front[1][0];
	int temp3 = front[2][0];

	front[0][0] = top[0][0];
	front[1][0] = top[1][0];
	front[2][0] = top[2][0];

	top[0][0] = back[2][2];
	top[1][0] = back[1][2];
	top[2][0] = back[0][2];

	back[2][2] = bottom[0][0];
	back[1][2] = bottom[1][0];
	back[0][2] = bottom[2][0];


	bottom[0][0] = temp1;
	bottom[1][0] = temp2;
	bottom[2][0] = temp3;
}
void _backc()
{
	_transpose('k');
	int temp1 = top[0][0];
	int temp2 = top[0][1];
	int temp3 = top[0][2];

	top[0][0] = right[0][2];
	top[0][1] = right[1][2];
	top[0][2] = right[2][2];

	right[0][2] = bottom[2][2];
	right[1][2] = bottom[2][1];
	right[2][2] = bottom[2][0];

	bottom[2][2] = left[2][0];
	bottom[2][1] = left[1][0];
	bottom[2][0] = left[0][0];

	left[2][0] = temp1;
	left[1][0] = temp2;
	left[0][0] = temp3;
}
void _bottomc()
{
	_transpose('b');
	int temp1 = front[2][0];
	int temp2 = front[2][1];
	int temp3 = front[2][2];

	front[2][0] = left[2][0];
	front[2][1] = left[2][1];
	front[2][2] = left[2][2];

	left[2][0] = back[2][0];
	left[2][1] = back[2][1];
	left[2][2] = back[2][2];

	back[2][0] = right[2][0];
	back[2][1] = right[2][1];
	back[2][2] = right[2][2];

	right[2][0] = temp1;
	right[2][1] = temp2;
	right[2][2] = temp3;

}

void _spincube()

{
	theta += 0.8+ speed;
	if (theta == 360.0)
		theta -= 360.0;
	if (theta >= 90.0)
	{
		rotationcomplete = 1;
		glutIdleFunc(NULL);

		if (rotation == 1 && inverse == 0)
		{
			_topc();
		}
		if (rotation == 1 && inverse == 1)
		{
			_topc();
			_topc();
			_topc();
		}

		if (rotation == 2 && inverse == 0)
		{
			_rightc();

		}
		if (rotation == 2 && inverse == 1)
		{
			_rightc();
			_rightc();
			_rightc();

		}
		if (rotation == 3 && inverse == 0)
		{
			_frontc();
		}
		if (rotation == 3 && inverse == 1)
		{
			_frontc();
			_frontc();
			_frontc();
		}
		if (rotation == 4 && inverse == 0)
		{
			_leftc();
		}
		if (rotation == 4 && inverse == 1)
		{
			_leftc();
			_leftc();
			_leftc();

		}
		if (rotation == 5 && inverse == 0)
		{
			_backc();
		}
		if (rotation == 5 && inverse == 1)
		{
			_backc();
			_backc();
			_backc();
		}
		if (rotation == 6 && inverse == 0)
		{
			_bottomc();
		}
		if (rotation == 6 && inverse == 1)
		{
			_bottomc();
			_bottomc();
			_bottomc();

		}

		rotation = 0;
		theta = 0;
	}



	glutPostRedisplay();
}

void _motion(int x, int y)
{
	if (moving) {

		q = q + (x - beginx);
		beginx = x;
		p = p + (y - beginy);
		beginy = y;
		glutPostRedisplay();
	}
}

void _mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		//printf("%d %d",x,y);

	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		/*printf("%d %d\n",x,y);
		if(x>=0&&x<=2&&y>=7&&y<=9)
		{
		printf("colour red\n");
		}
		*/
		moving = 1;
		beginx = x;
		beginy = y;

	}

}

///*static*/ void _keyboard(unsigned char key, int x, int y)
//{
//	if (key == 'a'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 1;
//		inverse = 0;
//		solve[++count] = 1;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 'q'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 1;
//		inverse = 1;
//		solve[++count] = -1;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 's'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 2;
//		inverse = 0;
//		solve[++count] = 2;
//		glutIdleFunc(_spincube);
//	}
//	if (key == 'w'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 2;
//		inverse = 1;
//		solve[++count] = -2;
//		glutIdleFunc(_spincube);
//	}
//	if (key == 'd'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 3;
//		inverse = 0;
//		solve[++count] = 3;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 'e'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 3;
//		inverse = 1;
//		solve[++count] = -3;
//		glutIdleFunc(_spincube);
//
//	}
//
//	if (key == 'f'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 4;
//		inverse = 0;
//		solve[++count] = 4;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 'r'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 4;
//		inverse = 1;
//		solve[++count] = -4;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 'g'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 5;
//		inverse = 0;
//		solve[++count] = 5;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == 't'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 5;
//		inverse = 1;
//		solve[++count] = -5;
//		glutIdleFunc(_spincube);
//
//	}
//
//	if (key == 'h'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 6;
//		inverse = 0;
//		solve[++count] = 6;
//		glutIdleFunc(_spincube);
//
//	}
//
//	if (key == 'y'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		rotation = 6;
//		inverse = 1;
//		solve[++count] = -6;
//		glutIdleFunc(_spincube);
//
//	}
//	if (key == '2'&&rotationcomplete == 1)
//	{
//		p = p + 2.0;
//		glutIdleFunc(_spincube);
//	}
//	if (key == '8'&&rotationcomplete == 1)
//	{
//		p = p - 2.0;
//		glutIdleFunc(_spincube);
//	}
//	if (key == '6'&&rotationcomplete == 1)
//	{
//		q = q + 2.0;
//		glutIdleFunc(_spincube);
//	}
//	if (key == '4'&&rotationcomplete == 1)
//	{
//		q = q - 2.0;
//		glutIdleFunc(_spincube);
//	}
//
//	if (key == '9'&&rotationcomplete == 1)
//	{
//		r = r + 2.0;
//		glutIdleFunc(_spincube);
//	}
//
//	if (key == '1'&&rotationcomplete == 1)
//	{
//		r = r - 2.0;
//		glutIdleFunc(_spincube);
//	}
//	if (key == '5'&&rotationcomplete == 1)
//	{
//		p = 0.0;
//		q = 0.0;
//		r = 0.0;
//		glutIdleFunc(_spincube);
//	}
//
//	if (key == 'm'&&rotationcomplete == 1)
//	{
//		if (speed <= 1.3)
//		{
//			//for(speed=0;speed<1.3;speed++)
//
//			speed = speed + 0.3;
//			speedmetercolor[++speedmetercount] = 3;
//
//		}
//		glutPostRedisplay();
//	}
//	if (key == 'm'&&rotationcomplete == 1)
//	{
//		if (speed>1.3)
//		{
//			if (speed <= 2.9)
//			{
//				//for(speed=0;speed<1.3;speed++)
//
//				speed = speed + 0.3;
//				speedmetercolor[++speedmetercount] = 4;
//			}
//		}
//		glutPostRedisplay();
//	}
//	if (key == 'm'&&rotationcomplete == 1)
//	{
//
//		if (speed>2.9)
//		{
//			if (speed <= 4.2)
//			{
//				//r(speed=0;speed<=4.3;speed+=0.1)
//				//{
//				speed = speed + 0.3;
//				speedmetercolor[++speedmetercount] = 5;
//			}
//		}
//		glutPostRedisplay();
//	}
//	if (key == 'n'&&rotationcomplete == 1)
//	{
//		if (speed >= 0.3)
//		{
//			speed = speed - 0.3;
//			speedmetercolor[speedmetercount--] = 0;
//		}
//		glutPostRedisplay();
//
//	}
//
//
//	if (key == 'o'&&rotationcomplete == 1)
//	{
//		rotationcomplete = 0;
//		if (count >= 0)
//		{
//			if (solve[count]<0)
//			{
//				rotation = -1 * solve[count];
//				inverse = 0;
//				glutIdleFunc(_spincube);
//			}
//			if (solve[count]>0)
//			{
//				rotation = solve[count];
//				inverse = 1;
//				glutIdleFunc(_spincube);
//			}
//
//
//			count--;
//		}
//
//
//		glutIdleFunc(_spincube);
//
//
//	}
//
//}

static void _keyboard(unsigned char key, int x, int y)
{
	if (key == 'b') {

		if (rotationcomplete == 1) {

			rotationcomplete = 0;

			switch (movesString[index]) {

			case 'U':
				if (movesString[index + 1] == '\'') {
					rotation = 1;
					inverse = 1;
					solve[++count] = -1;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {
					rotation = 1;
					inverse = 0;
					solve[++count] = 1;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;
				}


			case 'R':
				if (movesString[index + 1] == '\'') {
					rotation = 2;
					inverse = 1;
					solve[++count] = -2;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {
					rotation = 2;
					inverse = 0;
					solve[++count] = 2;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;

				}


			case 'F':
				if (movesString[index + 1] == '\'') {
					rotation = 3;
					inverse = 1;
					solve[++count] = -3;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {
					rotation = 3;
					inverse = 0;
					solve[++count] = 3;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;

				}


			case 'L':
				if (movesString[index + 1] == '\'') {
					rotation = 4;
					inverse = 1;
					solve[++count] = -4;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {

					rotation = 4;
					inverse = 0;
					solve[++count] = 4;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;
				}


			case 'B':
				if (movesString[index + 1] == '\'') {
					rotation = 5;
					inverse = 1;
					solve[++count] = -5;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {

					rotation = 5;
					inverse = 0;
					solve[++count] = 5;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;
				}

			case 'D':
				if (movesString[index + 1] == '\'') {
					rotation = 6;
					inverse = 1;
					solve[++count] = -6;
					glutIdleFunc(_spincube);
					index++;
					index++;
					index++;
					break;
				}
				else {

					rotation = 6;
					inverse = 0;
					solve[++count] = 6;
					glutIdleFunc(_spincube);
					index++;
					index++;
					break;
				}

			default:
				rotation = 0;
			}
		}

		if (key == 'p') { speed = speed + 1; }
		if (key == 'l') { speed = speed - 1; }

	}
}

void _myreshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-10.0, 10.0, -10.0*(GLfloat)h / (GLfloat)w, 10.0*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-10.0*(GLfloat)w / (GLfloat)h, 10.0*(GLfloat)w / (GLfloat)h, -10.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

void _mymenu(int id)
{
	if (rotationcomplete == 1)
	{
		rotationcomplete = 0;
		switch (id)
		{
		case 1:
			rotation = 1;
			inverse = 0;
			solve[++count] = 1;
			glutIdleFunc(_spincube);
			break;

		case 2:
			rotation = 1;
			inverse = 1;
			solve[++count] = -1;
			glutIdleFunc(_spincube);
			break;

		case 3:
			rotation = 2;
			inverse = 0;
			solve[++count] = 2;
			glutIdleFunc(_spincube);
			break;

		case 4:
			rotation = 2;
			inverse = 1;
			solve[++count] = -2;
			glutIdleFunc(_spincube);
			break;
		case 5:
			rotation = 3;
			inverse = 0;
			solve[++count] = 3;
			glutIdleFunc(_spincube);
			break;
		case 6:
			rotation = 3;
			inverse = 1;
			solve[++count] = -3;
			glutIdleFunc(_spincube);
			break;
		case 7:
			rotation = 4;
			inverse = 0;
			solve[++count] = 4;
			glutIdleFunc(_spincube);
			break;
		case 8:
			rotation = 4;
			inverse = 1;
			solve[++count] = -4;
			glutIdleFunc(_spincube);
			break;
		case 9:
			rotation = 5;
			inverse = 0;
			solve[++count] = 5;
			glutIdleFunc(_spincube);
			break;
		case 10:
			rotation = 5;
			inverse = 1;
			solve[++count] = -5;
			glutIdleFunc(_spincube);
			break;
		case 11:
			rotation = 6;
			inverse = 0;
			solve[++count] = 6;
			glutIdleFunc(_spincube);
			break;
		case 12:
			rotation = 6;
			inverse = 1;
			solve[++count] = -6;
			glutIdleFunc(_spincube);
			break;



		case 13:
			exit(0);
			break;

		}
	}

}

void drawRubiksCube::drawCube(int argc, char* argv[], /*char*/ int* colourArray, int size) {

	std::ofstream singMasterFile;

	//copy the colours for the scrambled cube.
	for (int i = 1; i <= size; i++) {
	
		_scrambledCubeColours[i] = colourArray[i];
	}
	_getFrontFaceColours(1);
	_getRightFaceColours(10);
	_getLeftFaceColours(19);
	_getBackFaceColours(28);
	_getTopFaceColours(37);
	_getBottomFaceColours(46);
	
	//call the singmaster generator
	_getSingmasterString(top[2][1]);
	_getSingmasterString(front[0][1]);
	_getSingmasterString(top[1][2]);
	_getSingmasterString(right[0][1]);
	_getSingmasterString(top[0][1]);
	_getSingmasterString(back[0][1]);
	_getSingmasterString(top[1][0]);
	_getSingmasterString(left[0][1]);
	
	_getSingmasterString(bottom[0][1]);
	_getSingmasterString(front[2][1]);
	_getSingmasterString(bottom[1][2]);
	_getSingmasterString(right[2][1]);
   	_getSingmasterString(bottom[2][1]);
	_getSingmasterString(back[2][1]);
	_getSingmasterString(bottom[1][0]);
	_getSingmasterString(left[2][1]);

	_getSingmasterString(front[1][2]);
	_getSingmasterString(right[1][0]);
	_getSingmasterString(front[1][0]);
	_getSingmasterString(left[1][2]);
	_getSingmasterString(back[1][0]);
	_getSingmasterString(right[1][2]);
	_getSingmasterString(back[1][2]);
	_getSingmasterString(left[1][0]);

	_getSingmasterString(top[2][2]);
	_getSingmasterString(front[0][2]);
	_getSingmasterString(right[0][0]);
	
	_getSingmasterString(top[0][2]);
	_getSingmasterString(right[0][2]);
	_getSingmasterString(back[0][0]);
	
	_getSingmasterString(top[0][0]);
	_getSingmasterString(back[0][2]);
	_getSingmasterString(left[0][0]);

	_getSingmasterString(top[2][0]);
	_getSingmasterString(left[0][2]);
	_getSingmasterString(front[0][0]);

	_getSingmasterString(bottom[0][2]);
	_getSingmasterString(right[2][0]);
	_getSingmasterString(front[2][2]);

	_getSingmasterString(bottom[0][0]);
	_getSingmasterString(front[2][0]);
	_getSingmasterString(left[2][2]);

	_getSingmasterString(bottom[2][0]);
	_getSingmasterString(left[2][0]);
	_getSingmasterString(back[2][2]);

	_getSingmasterString(bottom[2][2]);
	_getSingmasterString(back[2][0]);
	_getSingmasterString(right[2][2]);


	singMasterFile.open("singMasterData.txt");
	for (int i = 0; i < 48; i++) {
	
		singMasterFile << singMasterString[i];

		//put space after every two entries of the edges
		if (i <= 23) {
			if (i % 2 != 0) {
			
				singMasterFile << " ";
			}

		}

		else if (i==26||i==29||i==32||i==35||i==38||i==41||i==44) {
			singMasterFile << " ";
		}
	}

	singMasterFile.close();

	//run the optimal solver.
	system("OptimalCubeSolver.exe");

	//read the solution generated by the algorithm into a string
	std::ifstream moves;
	moves.open("Moves.txt");
	std::getline(moves, movesString);
	moves.close();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("RUBIK'S CUBE");
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(_myreshape);
	glutIdleFunc(_spincube);
	glutKeyboardFunc(_keyboard);
	//glutMouseFunc(_mouse);
	glutMotionFunc(_motion);
	//glutCreateMenu(_mymenu);
	/*glutAddMenuEntry("Top                   :a", 1);
	glutAddMenuEntry("Top Inverted    :q", 2);
	glutAddMenuEntry("Right                 :s", 3);
	glutAddMenuEntry("Right Inverted  :w", 4);
	glutAddMenuEntry("Front                 :d", 5);
	glutAddMenuEntry("Front Inverted  :e", 6);
	glutAddMenuEntry("Left                   :f", 7);
	glutAddMenuEntry("Left Inverted    :r", 8);
	glutAddMenuEntry("Back                  :g", 9);
	glutAddMenuEntry("Back Inverted   :t", 10);
	glutAddMenuEntry("Bottom              :h", 11);
	glutAddMenuEntry("Bottom Inverted :y", 12);

	glutAddMenuEntry("Exit", 13);
	glutAttachMenu(GLUT_RIGHT_BUTTON);*/
	/////////////////////////////////////
	//printf("%s", "what?");
	//_keyboard('q', 250, 25);
	//_keyboard('a', 250, 25);
	//_keyboard('w', 250, 25);
	//_keyboard('e', 250, 25);
	glutKeyboardFunc(_keyboard);
	glutDisplayFunc(_display);
	
	///////////////////////////////////////
	
	//glutDisplayFunc(_display);
	//glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	//return 0;
}



