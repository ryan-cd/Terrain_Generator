#ifndef __TERRAIN_GENERATOR_H__
#define __TERRAIN_GENERATOR_H__

using namespace std;
#include "Headers.h"

class TerrainGenerator{
public:
	TerrainGenerator(void); //constructor
	//getters
	vector<vector<float> > getTerrain();
	int getTerrainSize();
	

	//setters
	void setSize(int terrainSize);
	void setupTerrain();

	//misc
	void printTerrain(); //this will print the contents of the terrain array to the console
	void drawScene(); //draws the terrain

private:
	int terrainSize;
	float displacement;
	int faultIterations;
	vector<vector<float>> terrain; 
	bool firstLoad;
	//functions
	void drawQuad(int i, int j);


};

#endif