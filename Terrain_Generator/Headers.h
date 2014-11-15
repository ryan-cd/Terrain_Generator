/* This file contains every include needed for the whole project */

#ifndef __HEADERS_H__	//guard against cyclic dependancy
#define __HEADERS_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //needed for random seed
#include <iostream>
#include <vector>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
//This is set for a WINDOWS OS
//#  include <GL/gl.h>
//#  include <GL/glu.h>
//#  include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/GL.h>
#endif

#include "TerrainGenerator.h"

using namespace std;
#endif