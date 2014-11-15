#include "TerrainGenerator.h"



TerrainGenerator::TerrainGenerator(void)
{
	srand(time(NULL)); // set random number generator seed
	this->displacement = 0.1; // amount to increment or decrement a height
	this->faultIterations = 560;
}

//setters

void TerrainGenerator::setSize(int terrainSize)
{

	if (terrainSize >= 0){
		this->terrainSize = terrainSize;
		terrain.resize(terrainSize, vector<float>(terrainSize, 0));
	}
}

/*This function sets up the terrain hights
it is implemented from the psuedocode on this page
linked to from the assignment: 
http://www.lighthouse3d.com/opengl/terrain/index.php?impdetails
*/
void TerrainGenerator::setupTerrain()
{
	float v = 0, a = 0, b = 0, d = 0, c = 0, size = 0;
	size = this->terrainSize;
	d = sqrt(2 * (size*size));
	
	printf("v: %f, a: %f, b: %f, d: %f, c: %f\n", v, a, b, d, c);
	int numUp = 0; int numDown = 0;
	
	for (int iterations = 0; iterations < this->faultIterations; iterations++)
	{
		v = rand();
		a = sin(v);
		b = cos(v);
		c = ((float)rand() / (float)RAND_MAX) * d - d / 2;

		for (int i = 0; i < this->terrainSize; i++)
		{
			for (int j = 0; j < this->terrainSize; j++)
			{
				if (a * i + b * j - c > 0)
				{
					numUp++;
					if (this->terrain[i][j] + this->displacement <= 1)
						this->terrain[i][j] += this->displacement;
				}
				else
				{
					numDown++;
					if (this->terrain[i][j] - this->displacement >= 0)
						this->terrain[i][j] -= this->displacement;
				}
			}
		}
	}
	cout << "\nNumUP:" << numUp << ", NumDown: " << numDown << endl;
}
//getters
vector<vector<float> > TerrainGenerator::getTerrain()
{
	return this->terrain;
}

void TerrainGenerator::printTerrain()
{
	for (int i = 0; i < this->terrainSize; i++)
	{
		for (int j = 0; j < this->terrainSize; j++)
		{
			cout << this->terrain[i][j] << " ";
			if (j == this->terrainSize - 1)
				cout << "\n";
		}
	}
}

