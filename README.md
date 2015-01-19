# Terrain_Generator
OpenGL 1.0/C++ random terrain generation with dynamic lighting and shading. The program will prompt you for a size of terrain to generate, and then will generate a random terrain using an implementation of the [fault algorithm](http://www.lighthouse3d.com/opengl/terrain/index.php3?fault). 
 
The terrain is colored according to [hypsometric tinting](http://en.wikipedia.org/wiki/Hypsometric_tints).
 
The program defaults to display the terrain with [flat shading](http://en.wikipedia.org/wiki/Shading#Flat_shading), and the user is able to change the shading mode to [Gouraud shading](http://en.wikipedia.org/wiki/Shading#Gouraud_shading). 
 
The main terrain window opens along with a heightmap window that shows an overhead view of the terrain. The user can interact with the heightmap to instantly modify the terrain.
 
The user is also able to change the scene orientation, the amount and positioning of the lights, and turn on different kinds of wireframes.

![alt-text](http://i.imgur.com/9Jwbspb.png "Flat Shading")
![alt-text](http://i.imgur.com/5ACSdpS.png "Gouraud Shading")
![alt-text](http://i.imgur.com/2UU7j2Q.png "Multiple lights")

Required Libraries
---
glut.h, gl.h

Running the Program
---
This repository is a Visual Studio 2013 solution. An exe is in the debug folder. When running the program operation instructions will appear in the console window.
