#ifndef __TERRAIN_GENERATOR_H__
#define __TERRAIN_GENERATOR_H__

using namespace std;
#include "Headers.h"

class TerrainGenerator{
public:
	TerrainGenerator(void); //constructor
	//types
	enum FillMode { SOLID, WIREFRAME, COMBINATION };
	enum ColorMode { GREYSCALE, COLOR }; 
	enum ShadingMode { FLAT, GOURAUD };

	//getters
	vector<vector<float> > getTerrain(); //returns the data structure that holds the terrain
	int getTerrainSize(); //returns the terrain width (which equals the height)
	vector<float> normal(vector<float> a, vector<float> b); //returns of two vectors (normalized)


	//setters
	void setSize(int terrainSize); //set the terrain size record
	void setupTerrain(); //change the terrain data structure to hold a mountainous setup
	void setFillMode(FillMode newMode); //change the fill mode
	void setColorMode(ColorMode newMode);//change the color mode
	void setShadingMode(ShadingMode newMode); //change the shading mode
	void incrementHeight(int x, int y); //takes the coordinates of the terrain to change the height
	void decrementHeight(int x, int y); //takes the coordinates of the terrain to change the height

	//misc
	void drawScene(); //draws the terrain
	void drawHeightMap(); //draws the heightmap
	void reset(); //will make a new terrain
	void resetNormals(); //will redo all the normals 

private:
	float minHeight; //minimum height of a vertex
	float maxHeight; //max height of a vertex
	int terrainSize; //the internal store of the width of the terrain
	float displacement; //the amount to displace pixels at a fault
	int faultIterations; //the number of faults to make
	vector<vector<float> > terrain; //the data structure holding the terrain setup
	vector<vector<vector<float> > > faceNormalList; //the data structure holding the face normals
	vector<vector<vector<float> > > vertexNormalList; //the data structure holding the vertex normals
	bool firstLoad; //true if it has loaded already. false if it hasnt been loaded for the first time yet
	bool normalsDrawn; //whether the normals have been drawn

	FillMode fillMode; //the wirefram mode to use 
	ColorMode colorMode; //whether the colors are grey or colored
	ShadingMode shadingMode; //the active shading type

	//functions
	void drawQuad(int i, int j); //draw a quad using the point specified
	void setVertexColor(float height); // sets the color of the vertex based on its height

};

#endif