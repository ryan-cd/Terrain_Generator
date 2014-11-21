#ifndef __TERRAIN_GENERATOR_H__
#define __TERRAIN_GENERATOR_H__

using namespace std;
#include "Headers.h"

class TerrainGenerator{
public:
	TerrainGenerator(void); //constructor
	//types
	enum FillMode { SOLID, WIREFRAME, COMBINATION };

	//getters
	vector<vector<float> > getTerrain();
	int getTerrainSize();


	//setters
	void setSize(int terrainSize);
	void setupTerrain();
	void setFillMode(FillMode newMode);

	//misc
	void printTerrain(); //this will print the contents of the terrain array to the console
	void drawScene(); //draws the terrain
	void drawHeightMap(); //draws the heightmap
	void reset(); //will make a new terrain

private:
	int terrainSize;
	float displacement;
	int faultIterations;
	vector<vector<float>> terrain; 
	bool firstLoad; //true if it has loaded already. false if it hasnt been loaded for the first time yet

	FillMode fillMode; 
	//functions
	void drawQuad(int i, int j);


};

#endif