#include "Headers.h"



//Globals


float gCamPos[] = { 0, 50, 400 };	//where the camera is

float gSceneRotation[3] = { 0, 0, 0 }; //the rotation of the scene
float gMinSceneRotationX = 0, gMaxSceneRotationX = 90;
int gMinTerrainSize = 10, gMaxTerrainSize = 300;
unsigned int gFillMode = 0; //this is used in the code to toggle drawing modes
unsigned int gColorMode = 0; //this is used in the code to toggle fill mode
bool lighting = true; //whether the lights are on
bool gHeightmapDrawn = false; //whether the heightmap has been drawn already


int gWindowPositionX = 350, gWindowPositionY = 50; //where the first window appears
int gWindowSizeX = 800, gWindowSizeY = 800; //the size of the window
int gWindow2SizeX = gMaxTerrainSize, gWindow2SizeY = gMaxTerrainSize; //the size of the second window
int gWindow1 = 0, gWindow2 = 0; //specifies id of which window to work with

//lighting
float light_pos[] = { -100, -110, 0, 1.0 };

float amb0[4] = { 1, 1, 1, 1 };
float diff0[4] = { 1, 1, 1, 1 };
float spec0[4] = { 1, 1, 1, 1 };

float m_amb[] = { 0.1, 0.1, 0.1, 1.0 };
float m_diff[] = { 1, 1, 1, 1.0 };
float m_spec[] = { 1, 1, 1, 1.0 };
float shiny = 0.8f;

//lighting 2
float light_pos2[] = { 100, 0, 0, 1.0 };

float amb02[4] = { 1, 0, 0, 1 };
float diff02[4] = { 1, 0, 0, 1 };
float spec02[4] = { 1, 0, 0, 1 };

//Class instantiations
TerrainGenerator terrainGenerator;

/* Prompts user for inputs to run the program */
void promptUser()
{
	int terrainSize = 0;
	
	cout << "Welcome to Terrain Generator\n\n";

	cout << "\nControls:"
		<< "\n\nLEFT SCREEN:"
		<< "\nArrow keys to rotate scene"
		<< "\n+/- to zoom"
		<< "\nc to toggle coloring"
		<< "\nw to toggle wireframe mode"
		<< "\nr to reset"
		<< "\nq/esc to quit"
		<< "\n\nRIGHT SCREEN:"
		<< "\nLeft click on pixels to raise the \n corresponding mountain peak \n height at that point"
		<< "\nRight click on pixels to lower the \n corresponding mountain peak \n height";

	while (terrainSize < gMinTerrainSize || terrainSize > gMaxTerrainSize)
	{
		cout << "\n\nPlease enter a valid terrain size \nto generate (50-300) -> ";
		cin >> terrainSize;
	}
	terrainGenerator.setSize(terrainSize);
	terrainGenerator.setupTerrain();
	
}


/* OpenGL Keyboard Callbacks */
void keyboard(unsigned char key, int xIn, int yIn)
{
	//DELETE THIS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	vector<float> a = { 1, 1, 2 };
	vector<float> b = { 2, 4, 2 };
	switch (key)
	{
	case '_':
		gCamPos[2] += 10;
	case '-':
		gCamPos[2] += 10;
		break;
	case '=':
		gCamPos[2] -= 10;
	case '+':
		gCamPos[2] -= 10;
		break;
	case 'w':
		//toggle wireframe mode
		gFillMode++;
		if (gFillMode > 2)
			gFillMode = 0;

		switch (gFillMode)
		{
		case 0:
			terrainGenerator.setFillMode(TerrainGenerator::SOLID);
			break;
		case 1:
			terrainGenerator.setFillMode(TerrainGenerator::WIREFRAME);
			break;
		case 2:
			terrainGenerator.setFillMode(TerrainGenerator::COMBINATION);
			break;
		}
		
		break;
	case 'c':
		//toggle color mode
		gColorMode++;
		if (gColorMode > 1)
			gColorMode = 0;
		switch (gColorMode)
		{
		case 0:
			terrainGenerator.setColorMode(TerrainGenerator::COLOR);
			break;
		case 1:
			terrainGenerator.setColorMode(TerrainGenerator::GREYSCALE);
			break;
		}
		break;
	case 'r':
		terrainGenerator.reset();
		gHeightmapDrawn = false; //this will cause the heightmap to redraw
		break;
	case 'l':
		/* Toggle lighting */
		lighting = lighting ? false : true;
		if (lighting)
		{
			glEnable(GL_LIGHT0);
		}
		else 
		{
			glDisable(GL_LIGHT0);
		}
		break;
	case 's':
		glShadeModel(GL_SMOOTH);
		break;
	case 'f':
		glShadeModel(GL_FLAT);
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
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE); //enable backface culling

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 400);

	glShadeModel(GL_FLAT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

/* OpenGL Special Key Callback */
void special(int key, int x, int y)
{
	//the keys in the switch rotate the scene
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
		if (gSceneRotation[0] < gMaxSceneRotationX)
			gSceneRotation[0] += 1;
		break;

	case GLUT_KEY_DOWN:
		if (gSceneRotation[0] > gMinSceneRotationX)
			gSceneRotation[0] -= 1;
		break;

	case GLUT_KEY_F1:
		light_pos[0] -= 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;

	case GLUT_KEY_F2:
		light_pos[0] += 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;

	case GLUT_KEY_F3:
		light_pos[1] -= 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;

	case GLUT_KEY_F4:
		light_pos[1] += 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;

	case GLUT_KEY_F5:
		light_pos[2] -= 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;

	case GLUT_KEY_F6:
		light_pos[2] += 10;
		cout << endl << "Light 1 Position: " << light_pos[0] << " " << light_pos[1] << " " << light_pos[2];
		break;
	

	case GLUT_KEY_F7:
		light_pos2[0] -= 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;

	case GLUT_KEY_F8:
		light_pos2[0] += 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;

	case GLUT_KEY_F9:
		light_pos2[1] -= 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;

	case GLUT_KEY_F10:
		light_pos2[1] += 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;

	case GLUT_KEY_F11:
		light_pos2[2] -= 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;

	case GLUT_KEY_F12:
		light_pos2[2] += 10;
		cout << endl << "Light 2 Position: " << light_pos2[0] << " " << light_pos2[1] << " " << light_pos2[2];
		break;
	}

	glutPostRedisplay();
}

/* OpenGL Display function for main screen*/
void display1(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);

	gluLookAt(gCamPos[0], gCamPos[1], gCamPos[2], 0, 0, 0, 0, 1, 0);
	glPushMatrix(); // push scene rotation
	//rotate the scene by the amount specified by the user
	glRotatef(gSceneRotation[0], 1, 0, 0);
	glRotatef(gSceneRotation[1], 0, 1, 0);

	//move the terrain to be in the center of the scene
	if (terrainGenerator.getTerrainSize() >= gMinTerrainSize && terrainGenerator.getTerrainSize() <= gMaxTerrainSize)
		glTranslatef(-terrainGenerator.getTerrainSize() / 2, 0, -terrainGenerator.getTerrainSize() / 2);

	//draw the terrain
	terrainGenerator.drawScene();
	//glShadeModel(GL_SMOOTH);
	glPopMatrix();
	
	glutSwapBuffers();
}

void display2(void)
{
	//the heightmap only gets drawn once per terrain made
	if (!gHeightmapDrawn)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
	
		//The adjustments are to make the coordinates more natural before drawing
		glScalef(-1, -1, -1);
		glTranslatef(1, -1, 0);

		gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);
		
	
		terrainGenerator.drawHeightMap(); //draw the heightmap
		
		glPopMatrix();
	
		glutSwapBuffers();
		gHeightmapDrawn = true;
	}
}

/*OpenGL Display Callback
Will call display functions of both windows*/
void idle(void)
{
	glutSetWindow(gWindow1);
	glutPostRedisplay();
	glutSetWindow(gWindow2);
	glutPostRedisplay();
}

void mouse2(int button, int state, int x, int y)
{
	//calculate the indexes of the mountain array. the math correlates what is on screen to the array
	int i = round((float) x / gWindow2SizeX * terrainGenerator.getTerrainSize());
	int j = round((float)(gWindow2SizeY - y) / gWindow2SizeY * terrainGenerator.getTerrainSize());
	
	//either raise or lower the peak depending on if it was left/right clicked
	if (button == GLUT_LEFT_BUTTON)
		terrainGenerator.incrementHeight(i, terrainGenerator.getTerrainSize()-j);
	if (button == GLUT_RIGHT_BUTTON)
		terrainGenerator.decrementHeight(i, terrainGenerator.getTerrainSize() - j);
	
	//this will make the heightmap draw again
	gHeightmapDrawn = false;
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	promptUser(); //gives instructions and asks for size
	
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(gWindowSizeX, gWindowSizeY);
	glutInitWindowPosition(gWindowPositionX, gWindowPositionY);
	
	gWindow1 = glutCreateWindow("Terain Generator");	//creates the main window and sets its id
	
	/* lighting setup */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb02);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff02);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec02);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	glutDisplayFunc(display1);	//registers "display1" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	init();

	glutInitWindowSize(gWindow2SizeX, gWindow2SizeY);
	glutInitWindowPosition(gWindowSizeX + gWindowPositionX, gWindowPositionY);
	gWindow2 = glutCreateWindow("Height Map"); //set the id of the second window and creative 
	glutDisplayFunc(display2);
	glutIdleFunc(idle);
	glutMouseFunc(mouse2);

	glutMainLoop();				//starts the event glutMainLoop

	
	return(0);					//return may not be necessary on all compilers
}
