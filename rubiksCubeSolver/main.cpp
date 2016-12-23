//includes
#include "ExtractColours.h"
#include "drawRubiksCube.h"

int main(int argc,char* argv[]) {

	//this perform object performs the colours extraction part.
	ExtractColours extract;

	/*for (int i = 1; i <= 54; i++) {
		std::cout << "colour " << i << "= " << extract.Colours[i] << "\n";
	}*/

	//draw the rubiks cubes as per the colour detcted by the 'extract' object.
	drawRubiksCube rubiksCube;
	rubiksCube.drawCube(argc,argv,extract.Colours,54);
	
	return 0;
}
