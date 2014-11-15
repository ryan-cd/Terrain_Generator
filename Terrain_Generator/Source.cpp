#include "Headers.h"



//Globals
float gCamPos[] = { 0, 0, 3.42f };	//where the camera is

//Class instantiations
TerrainGenerator terrainGenerator;


void promptUser()
{
	int terrainSize = 0;
	cout << "Welcome to Terrain Generator\n\n";
	//UPDATE MIN SIZE TO 50
	//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
	while (terrainSize < 1 || terrainSize > 100)
	{
		cout << "Please enter a valid terrain size \nto generate (50-300) -> ";
		cin >> terrainSize;
	}
	terrainGenerator.setSize(terrainSize);
	terrainGenerator.setupTerrain();
	terrainGenerator.printTerrain();
}


//OpenGL functions
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
	case 'q':
	case 27:	//27 is the esc key
		exit(0);
		break;
	}
}

void init(void)
{
	
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(gCamPos[0], gCamPos[1], gCamPos[2], 0, 0, 0, 0, 1, 0);
	glColor3f(1, 1, 1);

	////draw the teapot
	glutSolidTeapot(1);

	//flush out to single buffer
	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	promptUser();
	
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);
	
	//glutCreateWindow("Terain Generator");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);

	init();

	
	cin >> gCamPos[0];
	glutMainLoop();				//starts the event glutMainLoop

	
	return(0);					//return may not be necessary on all compilers
}
