/*==============================================================
  This is a demo program for Fly2 System

  Hello !

  (C)2004-2012 Chuan-Chang Wang, All Rights Reserved
  Created : 0303, 2004, C. Wang

  Last Updated : 0904, 2012, C.Wang
 ===============================================================*/
#include "FlyWin32.h"
#include "Player.h"

using namespace std;
// some globals
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID, tID;
CHARACTERid actorID;            // the major character
ACTIONid idleID, runID;
TEXTid textID = FAILED_ID;
ROOMid terrainRoomID = FAILED_ID;

Player *player;

void QuitGame(BYTE, BOOL4);
void Update(int);
void Render(int);
void Input(BYTE, BOOL4);

void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);

/*------------------
  the main program
  C.Wang 0904, 2012
 -------------------*/
void FyMain(int argc, char **argv)
{
   // create a new world
   FyStartFlyWin32("Hello Fly2 !", 0, 0, 800, 600, FALSE);

   FySetModelPath("Data\\Models");
   FySetTexturePath("Data\\Scenes\\Textures");
   FySetShaderPath("Data\\Shaders");
   FySetScenePath("Data\\Scenes");

   // create a viewport
   vID = FyCreateViewport(0, 0, 800, 600);
   FnViewport vp;
   vp.ID(vID);
   vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

   // create a 3D scene & the 3D entities
   sID = FyCreateScene(1);

   // To load a scene file
   FnScene scene;
   scene.ID(sID);   // sID is the main scene you created using game world
   scene.Load("gameScene02");    // gameScene02.cw4

   // To load a terrain file
   tID = scene.CreateObject(OBJECT);
   FnObject terrain;
   terrain.ID(tID);
   terrain.Load("terrain");     // terrain.cw3
   terrain.Show(FALSE);         // hide the terrain

   // Create a room for terrain following, add the terrain to the room:
   terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
   FnRoom room;
   room.ID(terrainRoomID);
   room.AddObject(tID);

   nID = scene.CreateObject(OBJECT);
   cID = scene.CreateObject(CAMERA);
   lID = scene.CreateObject(LIGHT);

   //Player 
   player = new Player();
   player->LoadData(sID);
   player->Init(terrainRoomID);

   // translate the camera
   FnCamera camera;
   camera.ID(cID);
   camera.Rotate(X_AXIS, 90.0f, LOCAL);
   camera.Translate(0.0f, 70.0f, 400.0f, LOCAL);

   // translate the light
   FnLight light;
   light.ID(lID);
   light.SetName("MainLight");
   light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

   // set Hotkeys
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);
   FyDefineHotKey(FY_UP, Input, TRUE);        // Up for moving forward
   FyDefineHotKey(FY_RIGHT, Input, TRUE);   // Right for turning right
   FyDefineHotKey(FY_LEFT, Input, TRUE);     // Left for turning left
   FyDefineHotKey(FY_DOWN, Input, TRUE);     // Left for turning left

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   // bind a timer, frame rate = 30 fps
   FyBindTimer(0, 30.0f, Update, TRUE);
   FyBindTimer(1, 30.0f, Render, TRUE);

   // invoke the system
   FyInvokeFly(TRUE);
}


/*-------------------------------------------------------------------------------
  timer callback function which will be invoked by Fly2 System every 1/30 second
  C.Wang 0308, 2004
 --------------------------------------------------------------------------------*/
/*-------------------------------------------------------------
30fps timer callback in fixed frame rate for major game loop
--------------------------------------------------------------*/
void Update(int skip)
{

	player->Update(skip);
	
	// Homework #01 part 1
	// ....
}

/*----------------------
perform the rendering
C.Wang 0720, 2006
-----------------------*/
void Render(int skip)
{
	FnViewport vp;

	// render the whole scene
	vp.ID(vID);
	vp.Render3D(cID, TRUE, TRUE);

	// get camera's data
	FnCamera camera;
	camera.ID(cID);

	float pos[3], fDir[3], uDir[3];
	camera.GetPosition(pos);
	camera.GetDirection(fDir, uDir);

	// show frame rate
	static char string[128];
	if (frame == 0) {
		FyTimerReset(0);
	}

	if (frame / 10 * 10 == frame) {
		float curTime;

		curTime = FyTimerCheckTime(0);
		sprintf(string, "Fps: %6.2f", frame / curTime);
	}

	frame += skip;
	if (frame >= 1000) {
		frame = 0;
	}

	FnText text;
	text.ID(textID);

	text.Begin(vID);
	text.Write(string, 20, 20, 255, 0, 0);

	char posS[256], fDirS[256], uDirS[256];
	sprintf(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);

	text.Write(posS, 20, 35, 255, 255, 0);
	text.Write(fDirS, 20, 50, 255, 255, 0);
	text.Write(uDirS, 20, 65, 255, 255, 0);

	text.End();

	// swap buffer
	FySwapBuffers();
}


/*------------------
  quit the program
  C.Wang 0904, 2012
 -------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
   if (code == FY_ESCAPE) {
      if (value) {
         FyQuitFlyWin32();
      }
   }
}

/*------------------
   Input
-------------------*/
void Input(BYTE code, BOOL4 value)
{
	player->Input(code, value);
}


/*-----------------------------------
  initialize the pivot of the camera
  C.Wang 0329, 2004
 ------------------------------------*/
void InitPivot(int x, int y)
{
   oldX = x;
   oldY = y;
}


/*------------------
  pivot the camera
  C.Wang 0329, 2004
 -------------------*/
void PivotCam(int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.ID(cID);
      model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.ID(cID);
      model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
      oldY = y;
   }
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitMove(int x, int y)
{
   oldXM = x;
   oldYM = y;
}


/*------------------
  move the camera
  C.Wang 0329, 2004
 -------------------*/
void MoveCam(int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.ID(cID);
      model.Translate((float)(x - oldXM)*0.1f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, (float)(oldYM - y)*0.1f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitZoom(int x, int y)
{
   oldXMM = x;
   oldYMM = y;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2004
 -------------------*/
void ZoomCam(int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM), LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}