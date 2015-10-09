#include "Player.h"

void Player::LoadData(SCENEid sceneID)
{
	FnScene scene;
	scene.ID(sceneID);
	// To load a character file
	FySetModelPath("Data\\Characters");
	FySetTexturePath("Data\\Characters");
	FySetCharacterPath("Data\\Characters");
	actorID = scene.LoadCharacter("Lyubu2");

	
}
void Player::Init(ROOMid terrainRoomID)
{
	FnCharacter actor;
	actor.ID(actorID);
	actor.SetTerrainRoom(terrainRoomID, 10.0f); //Put the character on terrain

	float pos[3], fDir[3], uDir[3]; //Set the character position and facing direction & put it on terrain
	pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
	actor.SetDirection(fDir, uDir);
	BOOL4 beOK = actor.PutOnTerrain(pos);

	//set animation
	idleID = actor.GetBodyAction(NULL, "Idle");
	runID = actor.GetBodyAction(NULL, "Run");

	// set the character to idle action
	curPoseID = FAILED_ID;
	nextPosID = idleID;
	actor.TurnRight(90.0f);
}
void Player::Update(int skip)
{
	FnCharacter actor;

	actor.ID(actorID);

	if (speed != 0)
	{
		actor.MoveForward(5, FALSE, FALSE, 0, TRUE);
	}

	BehaviorUpdate(skip);

	AnimationUpdate(skip);

}

void Player::AnimationUpdate(int skip)
{
	FnCharacter actor;

	actor.ID(actorID);

	if (curPoseID != nextPosID)
	{
		actor.SetCurrentAction(NULL, 0, nextPosID);
		
	}
	actor.Play(LOOP, (float)skip, FALSE, TRUE);
	curPoseID = nextPosID;
}

void Player::BehaviorUpdate(int skip)
{
	FnCharacter actor;

	actor.ID(actorID);

	if (curPoseID == runID)
	{
		if (FyCheckHotKeyStatus(FY_UP) == FALSE)
		{
			nextPosID = idleID;
			speed = 0;
		}
	}
}

void Player::Render(int skip)
{
	
}
void Player::Input(BYTE code, BOOL4 value)
{
	FnCharacter actor;
	actor.ID(actorID);

	if (code == FY_UP){
		speed = 5;
		nextPosID = runID;
	}
	if (code == FY_RIGHT){
		actor.TurnRight(5);
	}
	if (code == FY_LEFT){
		actor.TurnRight(-5);
	}
	
}
