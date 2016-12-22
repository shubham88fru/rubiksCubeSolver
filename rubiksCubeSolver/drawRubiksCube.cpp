#include "drawRubiksCube.h"

/////////////////////////////////////////////////////////////
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
char defaultMessage[] = "Rotation Speed:";
char *message = defaultMessage;

static float speed = 0.0;

static int top[3][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } },
right[3][3] = { { 1,1,1 },{ 1,1,1 },{ 1,1,1 } },
front[3][3] = { { 2,2,2 },{ 2,2,2 },{ 2,2,2 } },
back[3][3] = { { 3,3,3 },{ 3,3,3 },{ 3,3,3 } },
bottom[3][3] = { { 4,4,4 },{ 4,4,4 },{ 4,4,4 } },
left[3][3] = { { 5,5,5 },{ 5,5,5 },{ 5,5,5 } },
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




GLfloat color[][3] = { { 1.0,1.0,1.0 },  //white
{ 1.0,0.5,0.0 },  //orange
{ 0.0,0.0,1.0 },  //blue
{ 0.0,1.0,0.0 },  //green
{ 1.0,1.0,0.0 },  //yellow
{ 1.0,0.0,0.0 }, //red
{ 0.1,0.1,0.1 }, //grey used to represent faces of cube without colour
{ .6,.5,.6 }//speed meter colour
};

//////////////////////////////////////////////////////////
drawRubiksCube::drawRubiksCube()
{
}

void drawRubiksCube::drawCube() {


}

void drawRubiksCube::_output(int x, int y, char* string) {

	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

void drawRubiksCube::_polygon(int a, int b, int c, int d, int e) {

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

void drawRubiksCube::_colorcube1()
{
	_polygon(6, 0, 3, 2, 1);
	_polygon(6, 2, 3, 7, 6);
	_polygon(6, 0, 4, 7, 3);    // center piece
	_polygon(6, 1, 2, 6, 5);
	_polygon(6, 4, 5, 6, 7);
	_polygon(6, 0, 1, 5, 4);


}
void drawRubiksCube::_colorcube2()
{
	//_polygon(6, 8, 11, 10, 9);
	//_polygon(6, 10, 11, 15, 14);
	//_polygon(6, 8, 12, 15, 11);    // bottom center
	//_polygon(6, 9, 10, 14, 13);
	//_polygon(6, 12, 13, 14, 15);
	//_polygon(bottom[1][1], 8, 9, 13, 12);


}

void drawRubiksCube::_colorcube3()
{

	//_polygon(6, 16, 19, 18, 17);
	//_polygon(6, 18, 19, 23, 22);
	//_polygon(left[1][1], 16, 20, 23, 19);    // left center
	//_polygon(6, 17, 18, 22, 21);
	//_polygon(6, 20, 21, 22, 23);
	//_polygon(6, 16, 17, 21, 20);


}

void drawRubiksCube::_colorcube4()
{
	//_polygon(6, 24, 27, 26, 25);
	//_polygon(6, 26, 27, 31, 30);
	//_polygon(6, 24, 28, 31, 27);    // right center
	//_polygon(right[1][1], 25, 26, 30, 29);
	//_polygon(6, 28, 29, 30, 31);
	//_polygon(6, 24, 25, 29, 28);


}

void drawRubiksCube::_colorcube5()
{
	//_polygon(6, 32, 35, 34, 33);
	//_polygon(top[1][1], 34, 35, 39, 38);
	//_polygon(6, 32, 36, 39, 35);    // top center
	//_polygon(6, 33, 34, 38, 37);
	//_polygon(6, 36, 37, 38, 39);
	//_polygon(6, 32, 33, 37, 36);


}

void drawRubiksCube::_colorcube6()
{
	_polygon(6, 40, 43, 42, 41);
	_polygon(6, 42, 43, 47, 46);
	_polygon(6, 40, 44, 47, 43);    // front center
	_polygon(6, 41, 42, 46, 45);
	_polygon(front[1][1], 44, 45, 46, 47);
	_polygon(6, 40, 41, 45, 44);


}

void drawRubiksCube::_colorcube7()
{
	_polygon(back[1][1], 48, 51, 50, 49);
	_polygon(6, 50, 51, 55, 54);
	_polygon(6, 48, 52, 55, 51);    //back center
	_polygon(6, 49, 50, 54, 53);
	_polygon(6, 52, 53, 54, 55);
	_polygon(6, 48, 49, 53, 52);


}


void drawRubiksCube::_colorcube8()
{
	_polygon(6, 56, 59, 58, 57);
	_polygon(top[1][0], 58, 59, 63, 62);
	_polygon(left[0][1], 56, 60, 63, 59);    // top left center
	_polygon(6, 57, 58, 62, 61);
	_polygon(6, 60, 61, 62, 63);
	_polygon(6, 56, 57, 61, 60);


}

void drawRubiksCube::_colorcube9()
{
	_polygon(6, 64, 67, 66, 65);
	_polygon(top[1][2], 66, 67, 71, 70);
	_polygon(6, 64, 68, 71, 67);    // top right center
	_polygon(right[0][1], 65, 66, 70, 69);
	_polygon(6, 68, 69, 70, 71);
	_polygon(6, 64, 65, 69, 68);


}

void drawRubiksCube::_colorcube10()
{
	_polygon(6, 72, 75, 74, 73);
	_polygon(top[2][1], 74, 75, 79, 78);
	_polygon(6, 72, 76, 79, 75);    // top front center
	_polygon(6, 73, 74, 78, 77);
	_polygon(front[0][1], 76, 77, 78, 79);
	_polygon(6, 72, 73, 77, 76);


}

void drawRubiksCube::_colorcube11()
{
	_polygon(back[0][1], 80, 83, 82, 81);
	_polygon(top[0][1], 82, 83, 87, 86);
	_polygon(6, 80, 84, 87, 83);    // top back center
	_polygon(6, 81, 82, 86, 85);
	_polygon(6, 84, 85, 86, 87);
	_polygon(6, 80, 81, 85, 84);


}

void drawRubiksCube::_colorcube12()
{
	_polygon(6, 80 + 8, 83 + 8, 82 + 8, 81 + 8);
	_polygon(6, 82 + 8, 83 + 8, 87 + 8, 86 + 8);
	_polygon(left[2][1], 80 + 8, 84 + 8, 87 + 8, 83 + 8);    // bottom left center
	_polygon(6, 81 + 8, 82 + 8, 86 + 8, 85 + 8);
	_polygon(6, 84 + 8, 85 + 8, 86 + 8, 87 + 8);
	_polygon(bottom[1][0], 80 + 8, 81 + 8, 85 + 8, 84 + 8);


}

void drawRubiksCube::_colorcube13()
{
	_polygon(6, 80 + 16, 83 + 16, 82 + 16, 81 + 16);
	_polygon(6, 82 + 16, 83 + 16, 87 + 16, 86 + 16);
	_polygon(6, 80 + 16, 84 + 16, 87 + 16, 83 + 16);    // bottom right center
	_polygon(right[2][1], 81 + 16, 82 + 16, 86 + 16, 85 + 16);
	_polygon(6, 84 + 16, 85 + 16, 86 + 16, 87 + 16);
	_polygon(bottom[1][2], 80 + 16, 81 + 16, 85 + 16, 84 + 16);


}

void drawRubiksCube::_colorcube14()
{
	_polygon(6, 80 + 24, 83 + 24, 82 + 24, 81 + 24);
	_polygon(6, 82 + 24, 83 + 24, 87 + 24, 86 + 24);
	_polygon(6, 80 + 24, 84 + 24, 87 + 24, 83 + 24);    // bottom front center
	_polygon(6, 81 + 24, 82 + 24, 86 + 24, 85 + 24);
	_polygon(front[2][1], 84 + 24, 85 + 24, 86 + 24, 87 + 24);
	_polygon(bottom[0][1], 80 + 24, 81 + 24, 85 + 24, 84 + 24);


}
void drawRubiksCube::_colorcube15()
{
	_polygon(back[2][1], 112, 115, 114, 113);
	_polygon(6, 114, 115, 119, 118);
	_polygon(6, 112, 116, 119, 115);    // bottom back center
	_polygon(6, 113, 114, 118, 117);
	_polygon(6, 116, 117, 118, 119);
	_polygon(bottom[2][1], 112, 113, 117, 116);


}

void drawRubiksCube::_colorcube16()
{
	_polygon(back[0][2], 120, 123, 122, 121);
	_polygon(top[0][0], 122, 123, 127, 126);
	_polygon(left[0][0], 120, 124, 127, 123);    // top left back
	_polygon(6, 121, 122, 126, 125);
	_polygon(6, 124, 125, 126, 127);
	_polygon(6, 120, 121, 125, 124);


}

void drawRubiksCube::_colorcube17()
{
	_polygon(6, 128, 131, 130, 129);
	_polygon(top[2][0], 130, 131, 135, 134);
	_polygon(left[0][2], 128, 132, 135, 131);    // top left front
	_polygon(6, 129, 130, 134, 133);
	_polygon(front[0][0], 132, 133, 134, 135);
	_polygon(6, 128, 129, 133, 132);


}


void drawRubiksCube::_colorcube18()
{
	_polygon(back[0][0], 136, 139, 138, 137);
	_polygon(top[0][2], 138, 139, 143, 142);
	_polygon(6, 136, 140, 143, 139);    // top right back
	_polygon(right[0][2], 137, 138, 142, 141);
	_polygon(6, 140, 141, 142, 143);
	_polygon(6, 136, 137, 141, 140);


}


void drawRubiksCube::_colorcube19()
{
	_polygon(6, 144, 147, 146, 145);
	_polygon(top[2][2], 146, 147, 151, 150);
	_polygon(6, 144, 148, 151, 147);    // top right front
	_polygon(right[0][0], 145, 146, 150, 149);
	_polygon(front[0][2], 148, 149, 150, 151);
	_polygon(6, 144, 145, 149, 148);


}

void drawRubiksCube::_colorcube20()
{
	_polygon(back[1][2], 152, 155, 154, 153);
	_polygon(6, 154, 155, 159, 158);
	_polygon(left[1][0], 152, 156, 159, 155);    //center left back
	_polygon(6, 153, 154, 158, 157);
	_polygon(6, 156, 157, 158, 159);
	_polygon(6, 152, 153, 157, 156);


}

void drawRubiksCube::_colorcube21()
{
	_polygon(6, 160, 163, 162, 161);
	_polygon(6, 162, 163, 167, 166);
	_polygon(left[1][2], 160, 164, 167, 163);    // center left front
	_polygon(6, 161, 162, 166, 165);
	_polygon(front[1][0], 164, 165, 166, 167);
	_polygon(6, 160, 161, 165, 164);


}


void drawRubiksCube::_colorcube22()
{
	_polygon(back[1][0], 168, 171, 170, 169);
	_polygon(6, 170, 171, 175, 174);
	_polygon(6, 168, 172, 175, 171);    // center right back
	_polygon(right[1][2], 169, 170, 174, 173);
	_polygon(6, 172, 173, 174, 175);
	_polygon(6, 168, 169, 173, 172);


}


void drawRubiksCube::_colorcube23()
{
	_polygon(6, 176, 179, 178, 177);
	_polygon(6, 178, 179, 183, 182);
	_polygon(6, 176, 180, 183, 179);    //center right front
	_polygon(right[1][0], 177, 178, 182, 181);
	_polygon(front[1][2], 180, 181, 182, 183);
	_polygon(6, 176, 177, 181, 180);


}

void drawRubiksCube::_colorcube24()
{
	_polygon(back[2][2], 184, 187, 186, 185);
	_polygon(6, 186, 187, 191, 190);
	_polygon(left[2][0], 184, 188, 191, 187);    // bottom left back
	_polygon(6, 185, 186, 190, 189);
	_polygon(6, 188, 189, 190, 191);
	_polygon(bottom[2][0], 184, 185, 189, 188);


}
void drawRubiksCube::_colorcube25()
{
	_polygon(6, 192, 195, 194, 193);
	_polygon(6, 194, 195, 199, 198);
	_polygon(left[2][2], 192, 196, 199, 195);    // bottom left front
	_polygon(6, 193, 194, 198, 197);
	_polygon(front[2][0], 196, 197, 198, 199);
	_polygon(bottom[0][0], 192, 193, 197, 196);


}

void drawRubiksCube::_colorcube26()
{
	_polygon(back[2][0], 200, 203, 202, 201);
	_polygon(6, 202, 203, 207, 206);
	_polygon(6, 200, 204, 207, 203);    // bottom right back
	_polygon(right[2][2], 201, 202, 206, 205);
	_polygon(6, 204, 205, 206, 207);
	_polygon(bottom[2][2], 200, 201, 205, 204);


}

void drawRubiksCube::_colorcube27()
{
	_polygon(6, 208, 211, 210, 209);
	_polygon(6, 210, 211, 215, 214);
	_polygon(6, 208, 212, 215, 211);    // bottom right front
	_polygon(right[2][0], 209, 210, 214, 213);
	_polygon(front[2][2], 212, 213, 214, 215);
	_polygon(bottom[0][2], 208, 209, 213, 212);


}
//drawRubiksCube::~drawRubiksCube()
//{
//}
