// --------------------------------------------------------------------------------
// Project Name		:	Platformer - Part 1
// File Name		:	BinaryMap.c
// Author			:	Damien Baca
// Creation Date	:	2015/2/24
// Purpose			:	Implementation of the binary map functions
// History			:
//	- 
//© Copyright 1996-2010, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------------


#include "BinaryMap.h"


/*The number of horizontal elements*/
static int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
static int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retireved from a file
when the "ImportMapDataFromFile" function is called*/
static int **MapData;

/*This will contain the collision dataof the binary map. It will be filled in the 
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element 
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
static int **BinaryCollisionArray;



int GetCellValue(unsigned int X, unsigned int Y)
{
	int value;

	if (X > (BINARY_MAP_WIDTH - 1) || X < 0)
		return 0;
	if (Y >(BINARY_MAP_HEIGHT - 1) || Y < 0)
	  return 0;

	value = BinaryCollisionArray[X][Y];

	return value;
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	int FLAG = 0;

	int RTX, RTY;  //Right Top X, Right Top Y
	int RBX, RBY;  //Right Bottom X, Right Bottom Y

	int LTX, LTY;  //Left Top X, Left Top Y
	int LBX, LBY;  //Left Bottom X, Left Bottom Y

	int TLX, TLY;  //Top Left X, Top Left Y
	int TRX, TRY;  //Top Right X, Top Right Y

	int BLX, BLY;  //Bottom Left X, Bottom Left Y
	int BRX, BRY;  //Bottom Right X, Bottom Right Y

// Right Side /////////////////////////////////////////////////////////////////

	//Right - Top
	RTX = PosX + scaleX / 2;
	RTY = PosY + scaleY / 4;

	//Right - Bottom
	RBX = PosX + scaleX / 2;
	RBY = PosY - scaleY / 4;

// Left Side //////////////////////////////////////////////////////////////////

	//Left - Top
	LTX = PosX - scaleX / 2;
	LTY = PosY + scaleY / 4;

	//Left - Bottom
	LBX = PosX - scaleX / 2;
	LBY = PosY - scaleY / 4;

// Top ////////////////////////////////////////////////////////////////////////

	//Top - Left
	TLX = PosX - scaleX / 4;
	TLY = PosY + scaleY / 2;

	//Top - Right
	TRX = PosX + scaleX / 4;
	TRY = PosY + scaleY / 2;


// Bottom /////////////////////////////////////////////////////////////////////

	//Bottom - Left
	BLX = PosX - scaleX / 4;
	BLY = PosY - scaleY / 2;

	//Bottom - Right
	BRX = PosX + scaleX / 4;
	BRY = PosY - scaleY / 2;

///////////////////////////////////////////////////////////////////////////////

	if (BinaryCollisionArray[(int)LTX][(int)LTY] == 1 || BinaryCollisionArray[(int)LBX][(int)LBY] == 1) //Checks Left Collision
		FLAG |= COLLISION_LEFT;
	
	if (BinaryCollisionArray[(int)RTX][(int)RTY] == 1 || BinaryCollisionArray[(int)RBX][(int)RBY] == 1) //Checks Right Collision
		FLAG |= COLLISION_RIGHT;

	if (BinaryCollisionArray[(int)TLX][(int)TLY] == 1 || BinaryCollisionArray[(int)TRX][(int)TRY] == 1) //Checks Top Collision
		FLAG |= COLLISION_TOP;
	
	if (BinaryCollisionArray[(int)BLX][(int)BLY] == 1 || BinaryCollisionArray[(int)BRX][(int)BRY] == 1) //Checks Bottom Collision 
		FLAG |= COLLISION_BOTTOM;

	return FLAG;
}

void SnapToCell(float *Coordinate)
{
	float temp;

	temp = (int)*Coordinate;

	temp += 0.5;

	*Coordinate = temp;
	
}

int ImportMapDataFromFile(char *FileName)
{
	int i = 0;
	int j = 0;
	int temp = 0;
	FILE *pfile;

	pfile = fopen(FileName, "rt");

	if (pfile == NULL)
	    return 0;

	fscanf_s(pfile, "Width %i\n", &BINARY_MAP_WIDTH);
	fscanf_s(pfile, "Height %i\n", &BINARY_MAP_HEIGHT);

	MapData = (int**)malloc( BINARY_MAP_WIDTH * sizeof (int*));
	BinaryCollisionArray = (int**)malloc(BINARY_MAP_WIDTH * sizeof(int*));

	for (i = 0; i < BINARY_MAP_WIDTH; i++)
		MapData[i] = (int*)malloc(BINARY_MAP_HEIGHT * sizeof(int));

	for (i = 0; i < BINARY_MAP_WIDTH; i++)
		BinaryCollisionArray[i] = (int*)malloc(BINARY_MAP_HEIGHT * sizeof(int));

	for (i = (BINARY_MAP_HEIGHT - 1); i >= 0; i--)
	{
		for (j = 0; j <= (BINARY_MAP_WIDTH - 1); j++)
		{
			fscanf_s(pfile, "%i", &MapData[j][i]);

			if (MapData[j][i] == 1)
				BinaryCollisionArray[j][i] = 1;
			else
				BinaryCollisionArray[j][i] = 0;
		}
	}

	fclose(pfile);
	return 1;
}

void FreeMapData(void)
{
	int i = 0;

	for (i = 0; i < BINARY_MAP_WIDTH; i++)
	{
		free(MapData[i]);
		free(BinaryCollisionArray[i]);
	}
	free(MapData);
	free(BinaryCollisionArray);
}

void PrintRetrievedInformation(void)
{
	int i, j;

	printf("Width: %i\n", BINARY_MAP_WIDTH);
	printf("Height: %i\n", BINARY_MAP_HEIGHT);

	printf("Map Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", MapData[i][j]);
		}
		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", BinaryCollisionArray[i][j]);
		}

		printf("\n");
	}
}