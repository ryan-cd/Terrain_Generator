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

	//getters
	vector<vector<float> > getTerrain();
	int getTerrainSize();


	//setters
	void setSize(int terrainSize);
	void setupTerrain();
	void setFillMode(FillMode newMode);
	void setColorMode(ColorMode newMode);
	void incrementHeight(int x, int y);
	void decrementHeight(int x, int y);

	//misc
	void printTerrain(); //this will print the contents of the terrain array to the console
	void drawScene(); //draws the terrain
	void drawHeightMap(); //draws the heightmap
	void reset(); //will make a new terrain

private:
	float minHeight; //minimum height of a vertex
	float maxHeight; //max height of a vertex
	int terrainSize;
	float displacement;
	int faultIterations;
	vector<vector<float>> terrain; 
	bool firstLoad; //true if it has loaded already. false if it hasnt been loaded for the first time yet

	FillMode fillMode; 
	ColorMode colorMode;

	//functions
	void drawQuad(int i, int j);
	void setVertexColor(float height); // sets the color of the vertex based on its height

};

#endif