#include "Headers.h"



//Globals
float gCamPos[] = { 0, 5, 200 };	//where the camera is
float gSceneRotation[3] = { 0, 0, 0 }; //the rotation of the scene

//Class instantiations
TerrainGenerator terrainGenerator;


void promptUser()
{
	int terrainSize = 0;
	cout << "Welcome to Terrain Generator\n\n";
	//UPDATE MIN SIZE TO 50
	//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
	while (terrainSize < 50 || terrainSize > 300)
	{
		cout << "Please enter a valid terrain size \nto generate (50-300) -> ";
		cin >> terrainSize;
	}
	cout << "\nWorking...";
	terrainGenerator.setSize(terrainSize);
	terrainGenerator.setupTerrain();
	
}


//OpenGL functions
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
	case'p':
		terrainGenerator.printTerrain();
		break;
	case 'q':
	case 27:	//27 is the esc key
		exit(0);
		break;
	}
}

void init(void)
{
	/* Setup GL features */
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //enable backface culling

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 200);
}

/* callbacks start here */
void special(int key, int x, int y)
{
	switch (key)
	{

		/* Arrow keys change the rotation of the scene*/
	case GLUT_KEY_RIGHT:
		gSceneRotation[1] -= 1;
		break;

	case GLUT_KEY_LEFT:
		gSceneRotation[1] += 1;
		break;

	case GLUT_KEY_UP:
		gSceneRotation[0] += 1;
		break;

	case GLUT_KEY_DOWN:
		gSceneRotation[0] -= 1;
		break;
	}
	glutPostRedisplay();
}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(gCamPos[0], gCamPos[1], gCamPos[2], 0, 0, 0, 0, 1, 0);
	glPushMatrix(); // push scene rotation
	//rotate the scene by the amount specified by the user
	glRotatef(gSceneRotation[0], 1, 0, 0);
	glRotatef(gSceneRotation[1], 0, 1, 0);

	glColor3f(1, 1, 1);

	terrainGenerator.drawScene();

	//flush out to single buffer
	glutSwapBuffers();
}

void idle(void)
{
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	promptUser();
	
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(350, 50);
	
	glutCreateWindow("Terain Generator");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	init();

	
	//cin >> gCamPos[0];
	glutMainLoop();				//starts the event glutMainLoop

	
	return(0);					//return may not be necessary on all compilers
}
