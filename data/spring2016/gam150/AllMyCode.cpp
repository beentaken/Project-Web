// GameStates.h needs to be before GameStateManager.h
#include "GameStates.h"
#include "GameStateManager.h"
#include "ae_lib/AEEngine.h"
#include "FPS.h"
int screen_width = 1000;
int screen_height = 1000;
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show
	// Window Creation
	// Initialize the system 
	AESysInitInfo sysInitInfo;
	int FPS_flag = FPS_OFF;
	sysInitInfo.mCreateWindow         = 1;
	sysInitInfo.mWindowHandle         = NULL;
	sysInitInfo.mAppInstance          = instanceH;
	sysInitInfo.mShow                 = show;
	sysInitInfo.mWinWidth             = 1000;
	sysInitInfo.mWinHeight            = screen_height;
	sysInitInfo.mCreateConsole        = 1;
	sysInitInfo.mMaxFrameRate         = 199;
	sysInitInfo.mpWinCallBack         = NULL;
	sysInitInfo.mClassStyle           = CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle          = WS_OVERLAPPEDWINDOW;
	sysInitInfo.mHandleWindowMessages = 1;
	AESysInit(&sysInitInfo);
	AESysReset();
    GSM_Initialize(InitialGameState);
    while(GameStateCurrent != Quit)
        if(GameStateCurrent != Restart)
            GSM_Update();
            pLoad(); 
        else
            GameStateNext = GameStateCurrent = GameStatePrevious;
        pInitialize();
        while(GameStateNext == GameStateCurrent)
			AESysFrameStart();
			if (FPS_flag == FPS_LOAD)
				FPS_Load();
				FPS_flag = FPS_ON;
			AEInputUpdate();
			if (AEInputCheckTriggered(VK_F10))
				FPS_flag = FPS_LOAD;
            pUpdate();
			if (FPS_flag == FPS_ON)
				FPS_Draw();
			pDraw();
			if (FPS_flag == FPS_UNLOAD)
				FPS_Unload();
				FPS_flag = FPS_OFF;
			AESysFrameEnd();
        pFree();
        if(GameStateNext != Restart)
            pUnload();
        GameStatePrevious = GameStateCurrent;
        GameStateCurrent = GameStateNext;
enum State Menu = 0, HighScore, Level,Pause, Restart, Quit, Andrei, Kacey, Sarah, Damien, Shawn;
void GSM_Initialize(int Initial);
void GSM_Update(void);
int GameStateNext;
int GameStateCurrent;
int GameStatePrevious;
int InitialGameState = Menu;
int GameStateNext;
int GameStateCurrent;
int GameStatePrevious;
void(*pLoad)(void);
void(*pInitialize)(void);
void(*pUpdate)(void);
void(*pDraw)(void);
void(*pFree)(void);
void(*pUnload)(void);
#include <stdio.h>
#include "GameStates.h"
#include "Menu.h"
#include "Pause.h"
#include "Level.h"
#include "Damien.h"
#include "Sarah.h"
#include "Kacey.h"
#include "Andrei.h"
extern int GameStateNext;
extern int GameStateCurrent;
extern int GameStatePrevious;
extern void(*pLoad)(void);
extern void(*pInitialize)(void);
extern void(*pUpdate)(void);
extern void(*pDraw)(void);
extern void(*pFree)(void);
extern void(*pUnload)(void);
void GSM_Initialize(int Initial
    GameStateCurrent = Initial;
    GameStateNext = Initial;
    GameStatePrevious = Initial;
void GSM_Update(void
    // Update functions for current gamestate
    // Some of these may be dummy functions.
    switch(GameStateCurrent)
        case Level:
            pLoad = &Load_Level;
            pInitialize = &Initialize_Level;
            pUpdate = &Update_Level;
            pDraw = &Draw_Level;
            pFree = &Free_Level;
            pUnload = &Unload_Level;
            break;
        case Pause:
            pLoad = &Load_Pause;
            pInitialize = &Initialize_Pause;
            pUpdate = &Update_Pause;
            pDraw = &Draw_Pause;
            pFree = &Free_Pause;
            pUnload = &Unload_Pause;
            break;
        case Menu:
            pLoad = &Load_Menu;
            pInitialize = &Initialize_Menu;
            pUpdate = &Update_Menu;
            pDraw = &Draw_Menu;
            pFree = &Free_Menu;
            pUnload = &Unload_Menu;
            break;
        case Andrei:
            pLoad = &Load_Andrei;
            pInitialize = &Initialize_Andrei;
            pUpdate = &Update_Andrei;
            pDraw = &Draw_Andrei;
            pFree = &Free_Andrei;
            pUnload = &Unload_Andrei;
            break;
        case Sarah:
            pLoad = &Load_Sarah;
            pInitialize = &Initialize_Sarah;
            pUpdate = &Update_Sarah;
            pDraw = &Draw_Sarah;
            pFree = &Free_Sarah;
            pUnload = &Unload_Sarah;
            break;
        case Damien:
            pLoad = &Load_Damien;
            pInitialize = &Initialize_Damien;
            pUpdate = &Update_Damien;
            pDraw = &Draw_Damien;
            pFree = &Free_Damien;
            pUnload = &Unload_Damien;
            break;
        case Kacey:
            pLoad = &Load_Kacey;
            pInitialize = &Initialize_Kacey;
            pUpdate = &Update_Kacey;
            pDraw = &Draw_Kacey;
            pFree = &Free_Kacey;
            pUnload = &Unload_Kacey;
            break;
#include <stdio.h>
#include "GameStates.h"
#include "Level.h"
#include "ae_lib/AEEngine.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Collision2D.h"uct
#include "Object.h"
#include "Sound.h"
#include "ScrollingBackground.h"
#define BUL_LIFETIME    2.0f
#define SHIP_ACCEL_FORWARD   30
#define SHIP_ACCEL_BACKWARD -30
#define SHIP_ACCEL_LEFT     -30
#define SHIP_ACCEL_RIGHT     30
#define SHIP_MAX_VEL         8
#define SHIP_DECEL          -5
#define SHIP_RELOAD_TIME     0.1f
#define SHIP_BUL_VELOCITY    10
#define ENEMY_BUL_VELOCITY   5
#define FRICTION_COEF       0.1f
#define INERTIA_COEF        0.4f
#define TRUE 1
#define FALSE 0
#define MAX_SOUNDS 10 // Idk, Arbitrary 
#define MAX_CHANNELS 32
extern int GameStateNext;
extern int screen_width;
extern int screen_height;
static GameInstance *player;
static List list;
static int reload = 0;
int PlayertoWall();
static FMOD_SYSTEM    *sys = NULL;
static FMOD_SOUND     *sounds[MAX_SOUNDS];
static FMOD_CHANNEL   *channel[MAX_CHANNELS];
static FMOD_RESULT     result;
void Load_Level(void
    AEGfxVertexList *pmesh;
    AEGfxTexture *pTex;
    memset(list.ObjectList, 0, sizeof(GameObj) * MAX_OBJECTS);
    list.Object_num = 0;
    memset(list.InstanceList, 0, sizeof(GameInstance) * MAX_INSTANCES);
    list.Instance_num = 0;
    AEGfxMeshStart();
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x00F00FFF, 0.0f, 0.0f);
    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);
    pmesh = AEGfxMeshEnd();
    pTex = AEGfxTextureLoad("Assets/Player_Ship.png");
    Create_Object(&list, pmesh, pTex, Player_Ship, Texture);
	pTex = AEGfxTextureLoad("Assets/Enemy_Ship.png");
	Create_Object(&list, pmesh, pTex, Enemy_Ship, Texture);
    pTex = AEGfxTextureLoad("Assets/Player_Bullet.png");
    Create_Object(&list, pmesh, pTex, Player_Bullet, Texture);
	pTex = AEGfxTextureLoad("Assets/Enemy_Bullet.png");
	Create_Object(&list, pmesh, pTex, Enemy_Bullet, Texture);
	ScrollingBackgroundLoad();
    sys = SoundSystemInit(sys, FMOD_INIT_NORMAL);
void Initialize_Level(void
	FMOD_RESULT res = FMOD_OK;
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	SetScrollingBackgroundSpeed(150);
	ScrollingBackgroundInit();
    Vector2D pos;
    Vector2DSet(&pos, 0.0f, -((float)screen_height/2 - 100));
    Vector2D vel;
    Vector2DSet(&vel, 0.0f, 0.0f);
    player = Create_Ship(&list, Player_Ship, 1.1f, 1.60f, &pos, &vel, 0, SHIP_RELOAD_TIME);/
    sounds[0] = CreateSound(sys, sounds[0], "Laser_Shoot.wav");
void Update_Level(void
    int i, p_bullets = 0, e_bullets = 0, enemy_num = 0;
    GameInstance *player_bullets[MAX_INSTANCES];
    GameInstance *enemy_bullets[MAX_INSTANCES];
    GameInstance *enemies[MAX_INSTANCES];
    Vector2D accel;
    Vector2D pos;
    Vector2D vel;
    float frametime;
	FMOD_System_Update(sys);
    frametime = (float)AEFrameRateControllerGetFrameTime();
	if (AEInputCheckTriggered(VK_F1))
		Vector2DSet(&pos, 550.0f, ((float)screen_height / 2 - 100));
		Vector2DSet(&vel, -3.0f, 0.0f);
		Create_Ship(&list, Enemy_Ship, 1.1f, 1.60f, &pos, &vel, PI, 1.0f);
	if (AEInputCheckTriggered(VK_F2))
		Vector2DSet(&pos, -550.0f, ((float)screen_height / 2 - 200));
		Vector2DSet(&vel, 3.0f, 0.0f);
		Create_Ship(&list, Enemy_Ship, 1.1f, 1.60f, &pos, &vel, PI, 0.5f);
    if (AEInputCheckCurr(VK_SPACE))
	    if (player->reload_ != NO_RELOAD)
	        if (player->reload_counter > player->reload_)
	           Vector2DSet(&vel, 0.0f, SHIP_BUL_VELOCITY);
	           Vector2DSet(&pos, player->position_.x, player->position_.y);
			   Create_Projectile(&list, Player_Bullet, 1.0f, 2.0f, &pos, &vel, 0, BUL_LIFETIME);
	           FMOD_System_PlaySound(sys, sounds[0], 0, FALSE, &channel[0]);
               player->reload_counter = 0;
			player->reload_counter++;
	if (AEInputCheckTriggered(VK_ESCAPE))
		GameStateNext = Menu;
    if (AEInputCheckCurr(VK_UP))
        Vector2DSet(&accel, 0.0f, SHIP_ACCEL_FORWARD);
        Vector2DScale(&accel, &accel, frametime);
        if (player->velocity_.y > SHIP_MAX_VEL)
            player->velocity_.y = SHIP_MAX_VEL;
        else
            Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
	if(AEInputCheckCurr(VK_DOWN))
        Vector2DSet(&accel, 0.0f, SHIP_ACCEL_BACKWARD);
        Vector2DScale(&accel, &accel, frametime);
        if (player->velocity_.y < -SHIP_MAX_VEL)
            player->velocity_.y = -SHIP_MAX_VEL;
        else
            Vecor2DAdd(&player->velocity_, &accel, &player->velocity_);
	if(AEInputCheckCurr(VK_RIGHT))
        Vector2DSet(&accel, SHIP_ACCEL_RIGHT, 0.0f);
        Vector2DScale(&accel, &accel, frametime);
        if (player->velocity_.x > SHIP_MAX_VEL)
            player->velocity_.x = SHIP_MAX_VEL;
        else
            Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
	if(AEInputCheckCurr(VK_LEFT))
        Vector2DSet(&accel, SHIP_ACCEL_LEFT, 0.0f);
        Vector2DScale(&accel, &accel, frametime);
        if (player->velocity_.x < -SHIP_MAX_VEL)
            player->velocity_.x = -SHIP_MAX_VEL;
        else
            Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
	if(!AEInputCheckCurr(VK_UP))
		Vector2DSet(&accel, 0.0f, -INERTIA_COEF);
		if (player->velocity_.y < -SHIP_MAX_VEL)
			player->velocity_.y = -SHIP_MAX_VEL;
		else
			Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
    if(!AEInputCheckCurr(VK_LEFT) && !AEInputCheckCurr(VK_RIGHT))
        Vector2DSet(&accel, -FRICTION_COEF * player->velocity_.x, 0.0f);
        Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
    if (!AEInputCheckCurr(VK_UP) && !AEInputCheckCurr(VK_DOWN))
        Vector2DSet(&accel, 0.0f, -FRICTION_COEF * player->velocity_.y);
        Vector2DAdd(&player->velocity_, &accel, &player->velocity_);
    PlayertoWall();
    // Iterating through all Instances
    for (i = 0; i < MAX_INSTANCES; i++)
        Matrix2D trans,
                 scale,
                 rot,
                 temp,
                 transform;
		GameInstance *Instance = &list.InstanceList[i];
        if (Instance->In_Use_ == FALSE)
            continue;
        Matrix2DScale(&scale, Instance->scale_x_, Instance->scale_y_);
        Matrix2DTranslate(&trans, Instance->position_.x, Instance->position_.y);
        Matrix2DRotRad(&rot, -Instance->rotation_);
        Matrix2DConcat(&temp, &rot, &scale);
        Matrix2DConcat(&transform, &trans, &temp);
        Instance->transform_ = transform;	
        Vector2DAdd(&Instance->position_, &Instance->position_, &Instance->velocity_);
        if (Instance->lifetime_ != NO_LIFETIME)
            if (Instance->lifetime_ > 0)
                Instance->lifetime_ -= 1;
            else
                Instance->In_Use_ = FALSE;//
        if (Instance->object_->Obj_Type_ == Player_Bullet)
            player_bullets[p_bullets] = Instance;
            p_bullets++;
        if (Instance->object_->Obj_Type_ == Enemy_Ship)
            enemies[enemy_num] = Instance;
            enemy_num++;
		if (Instance->object_->Obj_Type_ == Enemy_Ship)
			if (Instance->reload_ != NO_RELOAD)
				if (Instance->reload_counter > Instance->reload_)
					Vector2DSet(&vel, 0.0f, -ENEMY_BUL_VELOCITY);
					Vector2DSet(&pos, Instance->position_.x, Instance->position_.y);
					Create_Projectile(&list, Enemy_Bullet, 1.0f, 2.0f, &pos, &vel, PI, BUL_LIFETIME);
					Instance->reload_counter = 0;
				Instance->reload_counter++;
void Draw_Level(void
    int i;
	int z;
  ScrollingBackgroundDraw();
	for (z = Z_ORDER_VERYLOW; z < Z_ORDER_HIGH + 1; z++)
		for (i = 0; i < MAX_INSTANCES; ++i)
			GameInstance *Instance = &list.InstanceList[i];
			if (nstance->In_Use_ == FALSE)
				continue;
			if (Instance->z_order_ != z)
				continue;
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransform(Instance->transform_.m);
			AEGfxTextureSet(Instance->object_->pTex_, 0.0f, 0.0f);
			AEGfxMeshDraw(Instance->object_->pmesh_, AE_GFX_MDM_TRIANGLES);
void Free_Level(void
	FMOD_Sound_Release(sounds[0]);
	ScrollingBackgroundFree();
void Unload_Level(void
	FMOD_System_Close(sys);
	FMOD_System_Release(sys);
int PlayertoWall(
	if (player->position_.x >= (screen_width / 2) - 30)
		Vector2DSet(&player->velocity_, 0.0f, player->velocity_.y);
		Vector2DSet(&player->position_, (screen_width / 2) - 31.0f, player->position_.y);
    
	else if (player->position_.x <= -(screen_width / 2) + 30)
		Vector2DSet(&player->velocity_, 0.0f, player->velocity_.y);
		Vector2DSet(&player->position_, -(screen_width / 2) + 31.0f, player->position_.y);
	
	else if (player->position_.y >= (screen_height / 2) - 50)
		Vector2DSet(&player->velocity_, player->velocity_.x, 0.0f);
		Vector2DSet(&player->position_, player->position_.x, (screen_height / 2) - 51.0f);
	
	else if (player->position_.y <= -(screen_height / 2) + 50)
		Vector2DSet(&player->velocity_, player->velocity_.x, 0.0f);
		Vector2DSet(&player->position_, player->position_.x, -(screen_height / 2) + 51.0f);
	return 0;
void Matrix2DIdentity(Matrix2D *pResult
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = 0;
	
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx
	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[0][2] = pMtx->m[2][0];

	pResult->m[1][0] = pMtx->m[0][1];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[1][2] = pMtx->m[2][1];
	
	pResult->m[2][0] = pMtx->m[0][2];
	pResult->m[2][1] = pMtx->m[1][2];
	pResult->m[2][2] = pMtx->m[2][2];
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1
	int i;
	int j;
	struct Matrix2D Result;
	for (j = 0; j < 3; j++)
		for (i = 0; i < 3; i++)
			Result.m[j][i] = ((pMtx0->m[j][0] * pMtx1->m[0][i])
			                + (pMtx0->m[j][1] * pMtx1->m[1][i])
			                + (pMtx0->m[j][2] * pMtx1->m[2][i]));
	pResult->m[0][0] = Result.m[0][0];
	pResult->m[0][1] = Result.m[0][1];
	pResult->m[0][2] = Result.m[0][2];
					 
	pResult->m[1][0] = Result.m[1][0];
	pResult->m[1][1] = Result.m[1][1];
	pResult->m[1][2] = Result.m[1][2];
					
	pResult->m[2][0] = Result.m[2][0];
	pResult->m[2][1] = Result.m[2][1];
	pResult->m[2][2] = Result.m[2][2];
void Matrix2DTranslate(Matrix2D *pResult, float x, float y
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = x;

	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = y;

	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
void Matrix2DScale(Matrix2D *pResult, float x, float y
	pResult->m[0][0] = x;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;

	pResult->m[1][0] = 0;
	pResult->m[1][1] = y;
	pResult->m[1][2] = 0;

	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
void Matrix2DRotDeg(Matrix2D *pResult, float Angle

	pResult->m[0][0]  =  cosf(Angle * (PI/180));
	pResult->m[0][1]  =  sinf(Angle * (PI / 180));
	pResult->m[0][2]  =  0;		  
					  			  
	pResult->m[1][0]  = -sinf(Angle * (PI / 180));
	pResult->m[1][1]  = cosf(Angle * (PI / 180));
	pResult->m[1][2]  =  0;
					  
	pResult->m[2][0]  =  0;
	pResult->m[2][1]  =  0;
	pResult->m[2][2]  =  1;
void Matrix2DRotRad(Matrix2D *pResult, float Angle

	pResult->m[0][0] =  cosf(Angle);
	pResult->m[0][1] =  sinf(Angle);
	pResult->m[0][2] =  0;

	pResult->m[1][0] =  -sinf(Angle);
	pResult->m[1][1] =  cosf(Angle);
	pResult->m[1][2] =  0;

	pResult->m[2][0] =  0;
	pResult->m[2][1] =  0;
	pResult->m[2][2] =  1;
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec
	struct Vector2D Result;
	Result.x = (pMtx->m[0][0] * pVec->x) + (pMtx->m[0][1] * pVec->y) + (pMtx->m[0][2] * 1);
	Result.y = (pMtx->m[1][0] * pVec->x) + (pMtx->m[1][1] * pVec->y) + (pMtx->m[1][2] * 1);
	pResult->x = Result.x;
	pResult->y = Result.y;
#include "Vector2D.h"
#include "stdio.h"
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius
	float Length = sqrtf(((pCenter->x - pP->x) * (pCenter->x - pP->x))
		               + ((pCenter->y - pP->y) * (pCenter->y - pP->y)));
	if (Length <= Radius)
		return 1;
	return 0;
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height
	if (pPos->x < (pRect->x - (Width / 2)))
		return 0;
	
	else if (pPos->x > (pRect->x + (Width / 2)))
		return 0;
	
	else if (pPos->y > (pRect->x + (Height / 2)))
		return 0;
	
	else if (pPos->y < (pRect->x - (Height / 2)))
		return 0;
  return 1;
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1
	float Length = sqrtf(((pCenter0->x - pCenter1->x) * (pCenter0->x - pCenter1->x))
		                + ((pCenter0->y - pCenter1->y) * (pCenter0->y - pCenter1->y)));
	if (Length <= Radius0 + Radius1)
		return 1;
	return 0;
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1
	if ((pRect0->x + (Width0 / 2)) < (pRect1->x - (Width1 / 2)))
		return 0;
	
	else if ((pRect0->x - (Width0 / 2)) > (pRect1->x + (Width1 / 2)))
		return 0;
	
	else if ((pRect0->x - (Height0 / 2)) > (pRect1->x + (Height1 / 2)))
		return 0;
	
	else if ((pRect0->x + (Height0 / 2)) < (pRect1->x - (Height1 / 2)))
		return 0;
  return 1;
#include "Sound.h"
FMOD_SYSTEM *SoundSystemInit(FMOD_SYSTEM *system, FMOD_INITFLAGS flags
	void   *extradriverdata = 0;
    FMOD_System_Create(&system);
	FMOD_System_Init(system, MAX_CHANNELS, flags, extradriverdata);
	return system;
FMOD_SOUND * CreateSound(FMOD_SYSTEM *system, FMOD_SOUND *sound, char *source
	FMOD_System_CreateSound(system, source, FMOD_DEFAULT, 0, &sound);
	return sound;
#include "Object.h"
GameInstance* Create_Instance(List *list,         int type, 
                              float scale_x,      float scale_y,
                              Vector2D *position, Vector2D *velocity,
                              float rotation,     float lifetime,
	                          float reload,       int z_order
	int i;
	Vector2D zero  0.0f, 0.0f ;
	float frametime = (float)AEFrameRateControllerGetFrameTime();

	/* Find a game object Instance that is not in use*/
	for (i = 0; i < MAX_INSTANCES; i++)
		GameInstance *Instance = &list->InstanceList[i];

		if (Instance->In_Use_ == FALSE)

			Instance->object_    = get_object(list, type);
			Instance->In_Use_    = TRUE;
			Instance->scale_x_   = scale_x;
			Instance->scale_y_   = scale_y;
			Instance->position_  = position  ? *position : zero;
			Instance->velocity_  = velocity  ? *velocity : zero;
            Instance->rotation_  = rotation;
			Instance->z_order_   = z_order;

			if (lifetime == NO_LIFETIME)
				Instance->lifetime_ = NO_LIFETIME;
			else
				Instance->lifetime_ = lifetime / frametime;

			if (reload == NO_RELOAD)
				Instance->reload_ = NO_RELOAD;
			else
				Instance->reload_ = reload / frametime;

            list->Instance_num++;
			return Instance;
	printf("MAX INSTANCES REACHED");
	return 0;
GameInstance *Create_MenuButton(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation
	return Create_Instance(list, type, scale_x, scale_y, currpos, velocity, rotation, NO_LIFETIME, NO_RELOAD, Z_ORDER_LOW);
GameInstance *Create_Projectile(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation, float lifetime
	return Create_Instance(list, type, scale_x, scale_y, currpos, velocity, rotation, lifetime, NO_RELOAD, Z_ORDER_LOW);
GameInstance *Create_Ship(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation, float reload
	return Create_Instance(list, type, scale_x, scale_y, currpos, velocity, rotation, NO_LIFETIME, reload, Z_ORDER_MID);
GameObj *Create_Object(List *list, AEGfxVertexList *pmesh, AEGfxTexture *pTex, int Obj_Type, int Graph_Type
	GameObj *Object;
	Object = list->ObjectList + list->Object_num++;
	Object->Obj_Type_ = Obj_Type;
	Object->Graph_Type_ = Graph_Type;
	Object->pmesh_ = pmesh;
	Object->pTex_ = pTex;
	return Object;
GameObj *get_object(List *list, int type
	int i = 0;
	GameObj* obj = list->ObjectList;
	for (i = 0; i < list->Object_num; i++)
		if (list->ObjectList[i].Obj_Type_ == type)
			return &list->ObjectList[i];
	return NULL;
#include "Vector2D.h"
#include "Matrix2D.h"
#include "ae_lib\AEEngine.h"
#define MAX_OBJECTS 16
#define MAX_INSTANCES 1024
#define NO_LIFETIME         -1
#define NO_RELOAD           -1
#define FALSE 0
#define TRUE  1
enum FP FPS_Digit1, FPS_Digit2, FPS_Digit3;
enum LevelObject Player_Ship = 0, Player_Bullet, Enemy_Ship, Enemy_Bullet;
enum MenuObject Play_Button = 0, Exit_Button, Highscore_Button, Credits_Button, Title ;
enum GraphicTyp Mesh, Texture;
enum Z_ORDE Z_ORDER_VERYLOW, Z_ORDER_LOW, Z_ORDER_MID, Z_ORDER_HIGH, Z_ORDER_VERYHIGH ;
/* Game Object Struct */
typedef struc
    int Obj_Type_;           // Distinguishing Label of the object
    int Graph_Type_;         // Determines if we use pTex

	AEGfxVertexList* pmesh_; // The mesh of the object
    AEGfxTexture* pTex_;     // The texture of the object
GameObj;
/* Game Object Instance */
typedef struc
	GameObj* object_;       // A pointer to the corresponding object
	char In_Use_;           // Flag to determine wether an instance is present or not
	int z_order_;           // Order Instances are drawn in: [lowest -2 -1 0 1 2 Highest]
    float lifetime_;        // lifetime of the instance,      -1 for N/A
	float reload_;          // reload time of the instance,   -1 for N/a
	float scale_x_;         // The scale of the instance x
	float scale_y_;         // The scale of the instance y
	float rotation_;        // Rotation of the Instance
	Vector2D position_;     // The Current Position
	Vector2D velocity_;     // How much the instance moves in one frame
	Matrix2D transform_;    // Transforms the vertexes to World Space
	float reload_counter;   // Counter for reload
GameInstance;
typedef struc
	GameObj ObjectList[MAX_OBJECTS];
    GameInstance InstanceList[MAX_INSTANCES];
    int Object_num;
    int Instance_num;
List;/* Instance Creation is used internally */
GameInstance *Create_Instance(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation, float lifetime, float reload, int z_order);
GameInstance *Create_MenuButton(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation);
GameInstance *Create_Projectile(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation, float lifetime);
GameInstance *Create_Ship(List *list, int type, float scale_x, float scale_y, Vector2D *currpos, Vector2D *velocity, float rotation, float reload);
GameObj *Create_Object(List *list, AEGfxVertexList *pmesh, AEGfxTexture *pTex, int Obj_Type, int Graph_Type);
GameObj *get_object(List *list, int type);
#include "Object.h"
#define REFRESH_TIME 1
int i;
int refresh = 0;
List list;
extern int screen_height;
extern int screen_width;
static GameInstance *Digit1;
static GameInstance *Digit2;
static GameInstance *Digit3;
int FPS_Load(
	AEGfxVertexList *pmesh;
	AEGfxTexture *pTex;
	Vector2D pos;
	Vector2DSet(&pos, (screen_width/2 - 60), (screen_height / 2 - 30));
	memset(list.ObjectList, 0, sizeof(GameObj) * MAX_OBJECTS);
	list.Object_num = 0;
	memset(list.InstanceList, 0, sizeof(GameInstance) * MAX_INSTANCES);
	list.Instance_num = 0;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);
	pmesh = AEGfxMeshEnd();
	pTex = AEGfxTextureLoad("Assets/Disabled.png");
	Create_Object(&list, pmesh, pTex, FPS_Digit1, Texture);
	Create_Object(&list, pmesh, pTex, FPS_Digit2, Texture);
	Create_Object(&list, pmesh, pTex, FPS_Digit3, Texture);
	Digit1 = Create_MenuButton(&list, FPS_Digit1, 0.25f, 0.37f, &pos, 0, 0);
	pos.x += 20;
	Digit2 = Create_MenuButton(&list, FPS_Digit2, 0.25f, 0.37f, &pos, 0, 0);\
	pos.x += 20;
	Digit3 = Create_MenuButton(&list, FPS_Digit3, 0.25f, 0.37f, &pos, 0, 0);
	for (i = 0; i < 3; i++)	//*******************************************************************************//
		Matrix2D trans,
			scale,
			rot,
			temp,
			transform;
		GameInstance *Instance = &list.InstanceList[i];
		if (Instance->In_Use_ == FALSE)
			continue;
		// Scale
		Matrix2DScale(&scale, Instance->scale_x_, Instance->scale_y_);
		// Translate
		Matrix2DTranslate(&trans, Instance->position_.x, Instance->position_.y);
		// Rotate
		Matrix2DRotRad(&rot, -Instance->rotation_);
		// Build Transform
		Matrix2DConcat(&temp, &rot, &scale);
		Matrix2DConcat(&transform, &trans, &temp);
		// Set Transform
		Instance->transform_ = transform;
	return 0;
int FPS_Draw(
	int i;
	int hundreds;
	int tens;
	int ones;
	GameInstance *Instance;
	float frametime = (float)AEFrameRateControllerGetFrameTime();
	int fps = (int)(1.0f / frametime);
	hundreds = fps / 100;
	tens = (fps % 100) / 10;
	ones = (fps % 10);
if (refresh > REFRESH_TIME / frametime
	switch (tens)
	case 0:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Zero.png");
		break;
	case 1:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/One.png");
		break;
	case 2:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Two.png");
		break;
	case 3:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Three.png");
		break;
	case 4:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Four.png");
		break;
	case 5:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Five.png");
		break;
	case 6:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Six.png");
		break;
	case 7:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Seven.png");
		break;
	case 8:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Eight.png");
		break;
	case 9:
		Digit2->object_->pTex_ = AEGfxTextureLoad("Assets/Nine.png");
		break;
	switch (ones)
	case 0:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Zero.png");
		break;
	case 1:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/One.png");
		break;
	case 2:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Two.png");
		break;
	case 3:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Three.png");
		break;
	case 4:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Four.png");
		break;
	case 5:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Five.png");
		break;
	case 6:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Six.png");
		break;
	case 7:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Seven.png");
		break;
	case 8:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Eight.png");
		break;
	case 9:
		Digit3->object_->pTex_ = AEGfxTextureLoad("Assets/Nine.png");
		break;
	if (hundreds > 1)
		Digit1->object_->pTex_ = AEGfxTextureLoad("Assets/One.png");
	else
		Digit1->object_->pTex_ = NULL;

	refresh = 0;
refresh++;
	for (i = 0; i < MAX_INSTANCES; ++i)
		Instance = &list.InstanceList[i];
		if (Instance->In_Use_ == FALSE)
			continue;
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(Instance->transform_.m);
		AEGfxTextureSet(Instance->object_->pTex_, 0.0f, 0.0f);
		AEGfxMeshDraw(Instance->object_->pmesh_, AE_GFX_MDM_TRIANGLES);
	return 0;
int FPS_Unload(
	return 0;
