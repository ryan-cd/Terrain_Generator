#include "TerrainGenerator.h"



TerrainGenerator::TerrainGenerator(void)
{
	srand(time(NULL)); // set random number generator seed
	this->displacement = 0.02; // amount to increment or decrement a height
	this->faultIterations = 800;
	this->firstLoad = false;
	this->fillMode = SOLID;
}

void TerrainGenerator::drawScene(void)
{
	float height;
	for (int i = 0; i < this->terrainSize; i++)
	{
		for (int j = 0; j < this->terrainSize; j ++)
		{
			if (i+1 < this->terrainSize && j+1 < this->terrainSize)
				drawQuad(i, j);
		}
		
		
		
	}
	firstLoad = true;
}

void TerrainGenerator::drawQuad(int i, int j)
{
	float multiplier = 30; //multiplier to affect the height value
	float height = 0; //the height of the current vertex
	int iterations = 1; //if the fill mode is combination

	if (this->fillMode == SOLID) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		iterations = 1;
	}
	else if (this->fillMode == WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		iterations = 1;
	}
	else if (this->fillMode == COMBINATION)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		iterations = 2;
	}
	

	for (int z = 0; z < iterations; z++)
	{
		//this will only be reached if the mode is combo
		if (z == 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		
		glBegin(GL_QUADS);
		height = this->terrain[i][j];
		if (z==0)
			glColor3f(height, height, height);
		else
			glColor3f(1-height, 1-height, 1-height);
		glVertex3f(i, height * multiplier, j);

		height = this->terrain[i + 1][j];
		if (z == 0)
			glColor3f(height, height, height);
		else
			glColor3f(1 - height, 1 - height, 1 - height);
		glVertex3f(i + 1, height * multiplier, j);

		height = this->terrain[i + 1][j + 1];
		if (z == 0)
			glColor3f(height, height, height);
		else
			glColor3f(1 - height, 1 - height, 1 - height);
		glVertex3f(i + 1, height * multiplier, j + 1);

		height = this->terrain[i][j + 1];
		if (z == 0)
			glColor3f(height, height, height);
		else
			glColor3f(1 - height, 1 - height, 1 - height);
		glVertex3f(i, height * multiplier, j + 1);


		glEnd();

		
	}
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
	
	int numUp = 0; int numDown = 0;
	cout << endl << "completion...";
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
		
		if (!firstLoad && iterations % (this->faultIterations/10) == 0)
			 cout << (float)iterations / this->faultIterations * 100 << "%...";
	}
	//cout << "0,1" << terrain[0][1] << "2,3" << terrain[2][3];
	cout << "\nNumUP:" << numUp << ", NumDown: " << numDown << endl;
}

void TerrainGenerator::setFillMode(FillMode newMode)
{
	this->fillMode = newMode;
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

int TerrainGenerator::getTerrainSize(void)
{
	return this->terrainSize;
}

void TerrainGenerator::reset(void)
{
	this->firstLoad = false;
	setupTerrain();
}