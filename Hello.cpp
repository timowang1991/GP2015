/*==============================================================
  This is a demo program for Fly2 System

  Hello !

  (C)2004-2012 Chuan-Chang Wang, All Rights Reserved
  Created : 0303, 2004, C. Wang

  Last Updated : 0904, 2012, C.Wang
 ===============================================================*/
#include "FlyWin32.h"

using namespace std;

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID, tID, terrainRoomID, actorID;
ACTIONid idleID, runID;

void QuitGame(BYTE, BOOL4);
void GameAI(int);
void Movement(BYTE, BOOL4);

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

   // load the teapot model
   FnObject model;
   model.ID(nID);
   model.Load("teapot");

   // To load a character file
   FySetModelPath("Data\\Characters");
   FySetTexturePath("Data\\Characters");
   FySetCharacterPath("Data\\Characters");

   scene.ID(sID);
   actorID = scene.LoadCharacter("Lyubu2");

   FnCharacter actor;
   actor.ID(actorID);
   actor.SetTerrainRoom(terrainRoomID, 10.0f); //Put the character on terrain

   float pos[3], fDir[3], uDir[3]; //Set the character position and facing direction & put it on terrain
   pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
   fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
   uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
   actor.SetDirection(fDir, uDir);
   BOOL4 beOK = actor.PutOnTerrain(pos);

   idleID = actor.GetBodyAction(NULL, "Idle");
   runID = actor.GetBodyAction(NULL, "Run");

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
   FyDefineHotKey(FY_UP, Movement, TRUE);        // Up for moving forward
   FyDefineHotKey(FY_RIGHT, Movement, TRUE);   // Right for turning right
   FyDefineHotKey(FY_LEFT, Movement, TRUE);     // Left for turning left
   FyDefineHotKey(FY_DOWN, Movement, TRUE);     // Left for turning left

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   // bind a timer, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);

   // invoke the system
   FyInvokeFly(TRUE);
}


/*-------------------------------------------------------------------------------
  timer callback function which will be invoked by Fly2 System every 1/30 second
  C.Wang 0308, 2004
 --------------------------------------------------------------------------------*/
void GameAI(int skip)
{
   // render the scene
   FnViewport vp;
   vp.Object(vID);
   vp.Render3D(cID, TRUE, TRUE);

   int nV, nT;
   FyRenderBenchMark(&nT, &nV);

   FySwapBuffers();

   FnCharacter actor;
   actor.ID(actorID);
   actor.SetCurrentAction(0, NULL, idleID, 5.0f);
   actor.Play(START, 0.0f, FALSE, TRUE);
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
   Movement Control
-------------------*/
void Movement(BYTE code, BOOL4 value){
   FnCharacter actor;
   actor.ID(actorID);
   actor.SetCurrentAction(0, NULL, runID, 5.0f);
   actor.Play(START, 0.0f, FALSE, TRUE);

   if (code == FY_UP){
      actor.MoveForward(2, FALSE, FALSE, 0, TRUE);
   }
   else if (code == FY_RIGHT){
      actor.TurnRight(5);
   }
   else if (code == FY_LEFT){
      actor.TurnRight(-5);
   }
   else if (code == FY_DOWN){
      actor.MoveForward(-2, FALSE, FALSE, 0, TRUE);
   }
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