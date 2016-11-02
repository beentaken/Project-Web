// ---------------------------------------------------------------------------
// Project Name		:	Asteroid Game
// File Name		:	GameState_Play.cpp
// Author			:	Damien.Baca
// Creation Date	:	2015/02/15
// Purpose			:	implementation of the 'play' game state
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#include "main.h"
#include "Vector2D.h"
#include "Matrix2D.h"

// ---------------------------------------------------------------------------
// Defines

#define GAME_OBJ_NUM_MAX			32			//The total number of different objects (Shapes)
#define GAME_OBJ_INST_NUM_MAX		2048		//The total number of different game object instances


// Feel free to change these values in ordet to make the game more fun
#define SHIP_INITIAL_NUM			3			// initial number of ship lives
#define SHIP_SIZE					15.0f		// ship size
#define SHIP_ACCEL_FORWARD			80.0f		// ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			-40.0f		// ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// ship rotation speed (degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// homing missile rotation speed (degree/second)
#define BULLET_SPEED				500.0f		// bullet speed (m/s)

// ---------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_SHIP = 0, 
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_MISSILE,

	TYPE_NUM
};

// ---------------------------------------------------------------------------
// object flag definition

#define FLAG_ACTIVE		0x00000001

// ---------------------------------------------------------------------------
// Struct/Class definitions

//Game object structure
typedef struct 
{
	unsigned long			type;		// object type
	AEGfxVertexList*		pMesh;		// This will hold the triangles which will form the shape of the object
}GameObj;

// ---------------------------------------------------------------------------

//Game object instance structure
typedef struct 
{
	GameObj*			pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	Vector2D			posCurr;	// object current position
	Vector2D			velCurr;	// object current velocity
	float				dirCurr;	// object current direction

	Matrix2D		    transform;	// object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here
}GameObjInst;

// ---------------------------------------------------------------------------
// Static variables

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer ot the ship object
static GameObjInst*			spShip;										// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					sShipLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sScore;										// Current score

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
static GameObjInst*			gameObjInstCreate (unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
static void					gameObjInstDestroy(GameObjInst* pInst);

// ---------------------------------------------------------------------------

// "Load" function of this state
void GameStateAsteroidsLoad(void)
{
	GameObj* pObj;

	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	spShip = 0;

	// load/create the mesh data (game objects / Shapes)
	

	// =====================
	// create the ship shape
	// =====================

	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_SHIP;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFF0000, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f); 

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");


	// =======================
	// create the bullet shape
	// =======================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
	

	// =========================
	// create the asteroid shape
	// =========================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_ASTEROID;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xF4A46000, 0.0f, 0.0f,
		0.5f, -0.5f, 0xF4A46000, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xF4A46000, 0.0f, 0.0f,
		0.5f, 0.5f, 0xF4A46000, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");

	
	// ========================
	// create the missile shape
	// ========================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_ASTEROID;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
	
}

// ---------------------------------------------------------------------------

// "Initialize" function of this state
void GameStateAsteroidsInit(void)
{
	

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// create the main ship
	spShip = gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);

	Vector2D Velocity1;
	Vector2DSet(&Velocity1, 2.5f, 2.5f);

	Vector2D Velocity2;
	Vector2DSet(&Velocity2, -1.5f, -2.0f);

	Vector2D Velocity3;
	Vector2DSet(&Velocity3, -1.0f, 2.0f);

	Vector2D AsteroidPos1;
	Vector2DSet(&AsteroidPos1, 100.0f, 100.0f);

	gameObjInstCreate(TYPE_ASTEROID, 30.0f, &AsteroidPos1, &Velocity1, 0.0f);

	Vector2D AsteroidPos2;
	Vector2DSet(&AsteroidPos2, -100.0f, 100.0f);

	gameObjInstCreate(TYPE_ASTEROID, 30.0f, &AsteroidPos2, &Velocity2, 0.0f);

	Vector2D AsteroidPos3;
	Vector2DSet(&AsteroidPos3, 100.0f, -100.0f);

	gameObjInstCreate(TYPE_ASTEROID, 30.0f, &AsteroidPos3, &Velocity1, 0.0f);

	Vector2D AsteroidPos4;
	Vector2DSet(&AsteroidPos4, -100.0f, -100.0f);

	gameObjInstCreate(TYPE_ASTEROID, 30.0f, &AsteroidPos4, &Velocity2, 0.0f);
	
	// Create the initial asteroids instances using the "gameObjInstCreate" function

	// reset the score and the number of ship
	sScore      = 0;
	sShipLives    = SHIP_INITIAL_NUM;
}

// ---------------------------------------------------------------------------

// "Update" function of this state
void GameStateAsteroidsUpdate(void)
{
	unsigned long i;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	// ==========================================================================================
	// Getting the window's world edges (These changes whenever the camera moves or zooms in/out)
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	
	// ======================
	// Getting the frame time
	// ======================

	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// update according to input
	// =========================

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	Vector2D accel;

	if (AEInputCheckCurr(VK_UP))
	{
		Vector2DSet(&accel, (cosf(spShip->dirCurr) * SHIP_ACCEL_FORWARD), (sinf(spShip->dirCurr) * SHIP_ACCEL_FORWARD));
		Vector2DScale(&accel, &accel, frameTime);
		Vector2DAdd(&spShip->velCurr, &accel, &spShip->velCurr);
		Vector2DScale(&spShip->velCurr, &spShip->velCurr, 0.8f);
	}

	if (AEInputCheckCurr(VK_DOWN))
	{
		Vector2DSet(&accel, (cosf(spShip->dirCurr) * SHIP_ACCEL_BACKWARD), (sinf(spShip->dirCurr) * SHIP_ACCEL_BACKWARD));
		Vector2DScale(&accel, &accel, frameTime);
		Vector2DAdd(&spShip->velCurr, &accel, &spShip->velCurr);
		Vector2DScale(&spShip->velCurr, &spShip->velCurr, 0.8f);
	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		spShip->dirCurr += SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{
		spShip->dirCurr -= SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckTriggered(VK_SPACE))
	{
		Vector2D Velocity;
		Vector2DSet(&Velocity, (cosf(spShip->dirCurr) * BULLET_SPEED), (sinf(spShip->dirCurr) * BULLET_SPEED));
		Vector2DScale(&Velocity, &Velocity, frameTime);

		gameObjInstCreate(TYPE_BULLET, 10.0f, &spShip->posCurr, &Velocity, spShip->dirCurr);
	}	

	

	// Shoot a homing missle if 'm' is triggered (Create a new object instance)
	//if (AEInputCheckTriggered(DIK_M))


	// ==================================================
	// update physics of all active game object instances
	//	-- Positions are updated here
	// ==================================================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &pInst->velCurr);
	}
	

	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world (Needed for the assignment)
	//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
	//		-- Homing missile: compute its new orientation
	//		-- Update a particle effect (Not required for the Asteroids project)
	// ===================================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// check if the object is a ship
		if (pInst->pObject->type == TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
		}

		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			if (pInst->posCurr.x > winMaxX)
				pInst->posCurr.x = winMinX;
			if (pInst->posCurr.x < winMinX)
				pInst->posCurr.x = winMaxX;
			if (pInst->posCurr.y > winMaxY)
				pInst->posCurr.y = winMinY;
			if (pInst->posCurr.y < winMinY)
				pInst->posCurr.y = winMaxY;
		}
		
		


		// Remove bullets that go out of bounds


		// Update missile (Check if previous target is still alive, ajudst angle, find new target etc..)
	}


	// ====================
	// check for collision
	// ====================
	
	/*
	for each object instance: oi1
		if oi1 is not active
			skip

		if oi1 is an asteroid
			for each object instance oi2
				if(oi2 is not active or oi2 is an asteroid)
					skip

				if(oi2 is the ship)
					Check for collision between the ship and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a bullet)
					Check for collision between the bullet and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a missle)
					Check for collision between the missile and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
					*/
	float Shipx = spShip->scale;
	float Shipy = spShip->scale;

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pAsteroid = sGameObjInstList + i;

		// skip non-active object
		if ((pAsteroid->flag & FLAG_ACTIVE) == 0)
			continue;

		if (pAsteroid->pObject->type == TYPE_ASTEROID)
		{
			if (spShip->posCurr.x - spShip->scale <= pAsteroid->posCurr.x + pAsteroid->scale &&
				spShip->posCurr.x + spShip->scale >= pAsteroid->posCurr.x - pAsteroid->scale &&
				spShip->posCurr.y - spShip->scale <= pAsteroid->posCurr.y + pAsteroid->scale &&
				spShip->posCurr.y + spShip->scale >= pAsteroid->posCurr.y - pAsteroid->scale)
			{
				gameObjInstDestroy(spShip);
			}
			else
			{
				for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
				{
					GameObjInst* pBullet = sGameObjInstList + i;

					// skip non-active object
					if ((pBullet->flag & FLAG_ACTIVE) == 0 || pBullet == pAsteroid)
						continue;

					if (pBullet->pObject->type == TYPE_BULLET)
					{
						if (pBullet->posCurr.x - pBullet->scale <= pAsteroid->posCurr.x + pAsteroid->scale &&
							pBullet->posCurr.x + pBullet->scale >= pAsteroid->posCurr.x - pAsteroid->scale &&
							pBullet->posCurr.y - pBullet->scale <= pAsteroid->posCurr.y + pAsteroid->scale &&
							pBullet->posCurr.y + pBullet->scale >= pAsteroid->posCurr.y - pAsteroid->scale)
						{
							gameObjInstDestroy(pAsteroid);
							gameObjInstDestroy(pBullet);
						}
					}
				}
			}
		}
	}



	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		Matrix2D		 trans, rot, scale;
		GameObjInst* pInst = sGameObjInstList + i;
		

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		Matrix2DScale(&scale, pInst->scale, pInst->scale);

		// Compute the rotation matrix 
		Matrix2DRotRad(&rot, -pInst->dirCurr);

		// Compute the translation matrix
		Matrix2DTranslate(&trans, pInst->posCurr.x, pInst->posCurr.y);

		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		Matrix2D matrix1, matrix2;

		Matrix2DConcat(&matrix1, &scale, &rot);
		Matrix2DConcat(&matrix2, &trans, &matrix1);

		pInst->transform = matrix2;	

	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsDraw(void)
{
	char strBuffer[1024];
	unsigned long i;
	double frameTime;


	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// draw all object instances in the list

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// Already implemented. Explanation:
		// Step 1 & 2 are done outside the for loop (AEGfxSetRenderMode, AEGfxTextureSet, AEGfxSetTintColor) since all our objects share the same material.
		// If you want to have objects with difference materials (Some with textures, some without, some with transparency etc...)
		// then you'll need to move those functions calls inside the for loop
		// 1 - Set Render Mode (Color or texture)
		// 2 - Set all needed parameters (Color blend, textures, etc..)
		// 3 - Set the current object instance's transform matrix using "AEGfxSetTransform"
		// 4 - Draw the shape used by the current object instance using "AEGfxMeshDraw"

		AEGfxSetTransform(pInst->transform.m);
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsFree(void)
{
	int i = 0;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		gameObjInstDestroy(pInst);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsUnload(void)
{
	int i = 0;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEGfxMeshFree(pInst);
	}

	// free all mesh data (shapes) of each object using "AEGfxMeshFree"
}

// ---------------------------------------------------------------------------

GameObjInst* gameObjInstCreate(unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir)
{
	unsigned long i;
	Vector2D zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject	= sGameObjList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			
			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}
// ---------------------------------------------------------------------------