#include "TerrainGenerator.h"


//these vectors are used for the normal calculations 
vector<float> vertexA(3);
vector<float> vertexB(3);
vector<float> vertexC(3);
vector<float> vertexD(3);
vector<float> vectorAB(3);
vector<float> vectorBC(3);
vector<float> vectorCD(3);
vector<float> vectorDA(3);
vector<float> vectorBA(3);
vector<float> vectorCB(3);
vector<float> vectorDC(3);
vector<float> vectorAD(3);
vector<float> normalVectorA(3);
vector<float> normalVectorB(3);
vector<float> normalVectorC(3);
vector<float> normalVectorD(3);
vector<float> tempVector(3);

TerrainGenerator::TerrainGenerator(void)
{
	srand(time(NULL)); // set random number generator seed
	//NOTE: min and max height must not change from 0 or 1. 
	this->minHeight = 0; //min value allowed in the terrain data structure
	this->maxHeight = 1; //max value allowed in the terrain data structure
	this->displacement = 0.02; // amount to increment or decrement a height
	this->faultIterations = 800; //number of fault iterations to do
	this->firstLoad = false; //whether the terrain has already been loaded once already
	this->normalsDrawn = false;
	this->fillMode = SOLID; //wireframe mode
	this->colorMode = COLOR; //color/greyscale mode
}

void TerrainGenerator::drawScene(void)
{
	for (int i = 0; i < this->terrainSize; i++)
	{
		for (int j = 0; j < this->terrainSize; j ++)
		{
			if (i+1 < this->terrainSize && j+1 < this->terrainSize)
				drawQuad(i, j);
		}
	}
	//cout << endl << this->faceNormalList[0][0][0] << this->faceNormalList[0][0][1] << this->faceNormalList[0][0][2];
	firstLoad = true; //signify that the mountains loaded fully for a first time
	normalsDrawn = true;
}

void TerrainGenerator::drawHeightMap(void)
{
	glBegin(GL_POINTS);
	for (int i = 0; i < this->terrainSize; i++)
	{
		for (int j = 0; j < this->terrainSize; j++)
		{
			setVertexColor(this->terrain[i][j]);
			//the index is scaled to fit into the window
			glVertex3f((float)i / this->terrainSize*2, (float)j / this->terrainSize*2, 0);
		}
	}
	glEnd();
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
	
	//the for loop runs once usually. if its COMBINATION mode it runs twice
	for (int z = 0; z < iterations; z++)
	{
		/* The quad is indexed clockwise like this
		* A__B
		* |  |
		* D__C 
		* each upper left vertex will hold the normal for its face.
		* ie in the above diagram, the index corresponding with vertex
		* A will hold the normal of that face */


		//this conditional is only reached if the mode is combo
		if (z == 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glBegin(GL_QUADS);
		
		vertexA[0] = i; 
		vertexA[1] = this->terrain[i][j] * multiplier; 
		vertexA[2] = j;

		vertexB[0] = i + 1; 
		vertexB[1] = this->terrain[i+1][j] * multiplier; 
		vertexB[2] = j;

		vertexC[0] = i + 1; 
		vertexC[1] = this->terrain[i+1][j+1] * multiplier; 
		vertexC[2] = j + 1;

		vertexD[0] = i; 
		vertexD[1] = this->terrain[i][j+1] * multiplier; 
		vertexD[2] = j + 1;
		
		if (!normalsDrawn)
		{
			vectorAB = { vertexB[0] - vertexA[0], vertexB[1] - vertexA[1], vertexB[2] - vertexA[2] };
			vectorBC = { vertexC[0] - vertexB[0], vertexC[1] - vertexB[1], vertexC[2] - vertexB[2] };
			vectorCD = { vertexD[0] - vertexC[0], vertexD[1] - vertexC[1], vertexD[2] - vertexC[2] };
			vectorDA = { vertexD[0] - vertexA[0], vertexD[1] - vertexA[1], vertexD[2] - vertexA[2] };

			vectorBA = { vertexA[0] - vertexB[0], vertexA[1] - vertexB[1], vertexA[2] - vertexB[2] };
			vectorCB = { vertexB[0] - vertexC[0], vertexB[1] - vertexC[1], vertexB[2] - vertexC[2] };
			vectorDC = { vertexC[0] - vertexD[0], vertexC[1] - vertexD[1], vertexC[2] - vertexD[2] };
			vectorAD = { vertexA[0] - vertexD[0], vertexA[1] - vertexD[1], vertexA[2] - vertexD[2] };
		
			normalVectorA = normal(vectorAB, vectorAD);
			normalVectorB = normal(vectorBA, vectorBC);
			normalVectorC = normal(vectorCB, vectorCD);
			normalVectorD = normal(vectorDC, vectorDA);
			
			tempVector = { ((float)normalVectorA[0] + normalVectorB[0] + normalVectorC[0] + normalVectorD[0]) / 4,
				((float)normalVectorA[1] + normalVectorB[1] + normalVectorC[1] + normalVectorD[1]) / 4,
				((float)normalVectorA[2] + normalVectorB[2] + normalVectorC[2] + normalVectorD[2]) / 4 };

			this->faceNormalList[i][j] = tempVector;

			//in this case there is enough information to calculate a vertex normal
			if (i >= 1 && j >= 1)
			{
				//set the vertex normal
				for (int q = 0; q < 3; q++)
				{
					this->vertexNormalList[i][j][q] = 
						(float)(faceNormalList[i - 1][j - 1][q] +
						faceNormalList[i][j - 1][q] +
						faceNormalList[i][j][q] +
						faceNormalList[i - 1][j][q]) / 4;

					this->vertexNormalList[i+1][j][q] =
						(float)(faceNormalList[i][j - 1][q] +
						faceNormalList[i+1][j - 1][q] +
						faceNormalList[i+1][j][q] +
						faceNormalList[i][j][q]) / 4;

					this->vertexNormalList[i + 1][j+1][q] =
						(float)(faceNormalList[i][j][q] +
						faceNormalList[i + 1][j][q] +
						faceNormalList[i + 1][j+1][q] +
						faceNormalList[i][j+1][q]) / 4;

					this->vertexNormalList[i][j + 1][q] =
						(float)(faceNormalList[i-1][j][q] +
						faceNormalList[i][j][q] +
						faceNormalList[i][j + 1][q] +
						faceNormalList[i-1][j + 1][q]) / 4;
				}
			}
		}
		//set the normal of this set of points
		if (this->shadingMode == FLAT)
		{
			glNormal3f(this->faceNormalList[i][j][0],
				this->faceNormalList[i][j][1],
				this->faceNormalList[i][j][2]);
		}
		
		
		// DRAW VERTEX A //
		if (this->shadingMode == GOURAUD)
		{
			glNormal3f(this->vertexNormalList[i][j][0],
				this->vertexNormalList[i][j][1],
				this->vertexNormalList[i][j][2]);
		}

		if (z == 0)
			setVertexColor((float)vertexA[1] / multiplier);
		else
		{
			glColor3f(1 - (float)vertexA[1] / multiplier,
				1 - (float)vertexA[1] / multiplier,
				1 - (float)vertexA[1] / multiplier);
		}
		glVertex3f(vertexA[0], vertexA[1], vertexA[2]);
		

		// DRAW VERTEX B //
		if (this->shadingMode == GOURAUD)
		{
			glNormal3f(this->vertexNormalList[i + 1][j][0], 
				this->vertexNormalList[i + 1][j][1],
				this->vertexNormalList[i + 1][j][2]);
		}
		if (z == 0)
			setVertexColor((float)vertexB[1] / multiplier);
		else
		{
			glColor3f(1 - (float)vertexB[1] / multiplier,
				1 - (float)vertexB[1] / multiplier,
				1 - (float)vertexB[1] / multiplier);
		}
		glVertex3f(vertexB[0], vertexB[1], vertexB[2]);
		

		// DRAW VERTEX C //
		if (this->shadingMode == GOURAUD)
		{
			glNormal3f(this->vertexNormalList[i + 1][j + 1][0],
				this->vertexNormalList[i + 1][j + 1][1],
				this->vertexNormalList[i + 1][j + 1][2]);
		}
		if (z == 0)
			setVertexColor((float)vertexC[1] / multiplier);
		else
		{
			glColor3f(1 - (float)vertexC[1] / multiplier,
				1 - (float)vertexC[1] / multiplier,
				1 - (float)vertexC[1] / multiplier);
		}
		glVertex3f(vertexC[0], vertexC[1], vertexC[2]);
		

		// DRAW VERTEX D //
		if (this->shadingMode == GOURAUD)
		{
			glNormal3f(this->vertexNormalList[i][j + 1][0],
				this->vertexNormalList[i][j + 1][1],
				this->vertexNormalList[i][j + 1][2]);
		}

		if (z == 0)
			setVertexColor((float)vertexD[1] / multiplier);
		else
		{
			glColor3f(1 - (float)vertexD[1] / multiplier,
				1 - (float)vertexD[1] / multiplier,
				1 - (float)vertexD[1] / multiplier);
		}
		glVertex3f(vertexD[0], vertexD[1], vertexD[2]);
		

		glEnd();
	}
}

/* Private function 
Sets the GLColor call*/
void TerrainGenerator::setVertexColor(float height)
{
	float color[3] = { 0, 0, 0 }; // the color to set the vertex if it is color mode

	if (colorMode == GREYSCALE) 
		glColor3f(height, height, height);
	else if (colorMode == COLOR)
	{
		if (height <= (float)this->maxHeight / 4)
		{
			color[0] = 0;
			color[1] = (float)height / ((float)this->maxHeight / 4);
			color[2] = 0;
			glColor3fv(color);
			return;
		}
		else if (height <= (float)this->maxHeight / 2)
		{
			color[0] = (float)height / ((float)this->maxHeight / 2);
			color[1] = 1;
			color[2] = 0;
			glColor3fv(color);
			return;
		}
		else if (height <= (float)this->maxHeight / 4 * 3)
		{
			color[0] = 1;
			color[1] = (float)((float)this->maxHeight / 4 * 3) - height;
			color[2] = 0;
			glColor3fv(color);
			return;
		}
		else
		{
			color[0] = (float) height / this->maxHeight;
			color[1] = color[0];
			color[2] = color[0];
			glColor3fv(color);
			return;
		}
	}
}

//setters

void TerrainGenerator::setSize(int terrainSize)
{

	if (terrainSize >= 0){
		this->terrainSize = terrainSize;
		terrain.resize(terrainSize, vector<float>(terrainSize, 0));

		faceNormalList.resize(terrainSize);
		vertexNormalList.resize(terrainSize);
		for (int i = 0; i < terrainSize; i++)
		{
			faceNormalList[i].resize(terrainSize);
			vertexNormalList[i].resize(terrainSize);
			for (int j = 0; j < terrainSize; j++)
			{
				faceNormalList[i][j].resize(3);
				vertexNormalList[i][j].resize(3);
			}
		}
	}
}

/*This function sets up the terrain heights
it is implemented from the psuedocode on this page
linked to from the assignment: 
http://www.lighthouse3d.com/opengl/terrain/index.php?impdetails
*/
void TerrainGenerator::setupTerrain()
{
	float v = 0, a = 0, b = 0, d = 0, c = 0, size = 0;
	size = this->terrainSize;
	d = sqrt(2 * (size*size));
	
	cout << "\r                                         "; // clear the console line
	for (int iterations = 1; iterations <= this->faultIterations; iterations++)
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
					if (this->terrain[i][j] + this->displacement <= this->maxHeight)
						this->terrain[i][j] += this->displacement;
				}
				else
				{
					if (this->terrain[i][j] - this->displacement >= this->minHeight)
						this->terrain[i][j] -= this->displacement;
				}
			}
			
		}
		
		if (!firstLoad && iterations % (this->faultIterations/10) == 0)
			 cout << "\rLoading..." << (float)iterations / (this->faultIterations) * 100 << "%";
	}
	cout << "\rLoading...Done!";
	
}

void TerrainGenerator::setFillMode(FillMode newMode)
{
	this->fillMode = newMode;
}

void TerrainGenerator::setColorMode(ColorMode newMode)
{
	this->colorMode = newMode;
}

void TerrainGenerator::setShadingMode(ShadingMode newMode)
{
	this->shadingMode = newMode;
}

void TerrainGenerator::incrementHeight(int x, int y)
{
	if (0 <= x && x <= this->terrainSize
		&& 0 <= y && y <= this->terrainSize
		&& this->terrain[x][y] + this->displacement <= this->maxHeight)
	{
		this->terrain[x][y] += this->displacement;
	}
}
void TerrainGenerator::decrementHeight(int x, int y)
{
	if (0 <= x && x <= this->terrainSize
		&& 0 <= y && y <= this->terrainSize
		&& this->terrain[x][y] - this->displacement >= this->minHeight)
	{
		this->terrain[x][y] -= this->displacement;
	}
}



//getters
vector<vector<float> > TerrainGenerator::getTerrain()
{
	return this->terrain;
}

int TerrainGenerator::getTerrainSize(void)
{
	return this->terrainSize;
}

/* inputs two 3d vectors returns the normalized normal */
vector<float> TerrainGenerator::normal(vector<float> a, vector<float> b)
{
	//add input checking
	vector<float> result(3);
	float magnitude;
	result[0] = (float)a[1] * b[2] - a[2] * b[1];
	result[1] = (float)a[2] * b[0] - a[0] * b[2];
	result[2] = (float)a[0] * b[1] - a[1] * b[0];
	magnitude = sqrt(pow(result[0], 2) + pow(result[1],2) + pow(result[2], 2));

	for (int i = 0; i <= 2; i++)
		result[i] /= magnitude;
	
	//cout << "\n" << result[0] << " " << result[1] << " " << result[2];

	return result;
}

void TerrainGenerator::reset(void)
{
	this->firstLoad = false;
	setupTerrain();
}

void TerrainGenerator::resetNormals(void)
{
	this->normalsDrawn = false;
}