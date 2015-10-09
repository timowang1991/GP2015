#include "FlyWin32.h"

class Player
{
protected:
	int actorID;
	ACTIONid idleID, runID, curPoseID, nextPosID;
	int speed = 0;
	void AnimationUpdate(int skip);
	void BehaviorUpdate(int skip);
	
public:
	void LoadData(SCENEid sceneID);
	void Init(ROOMid terrainRoomID);
	void Update(int skip);
	void Render(int skip);
	void Input(BYTE code, BOOL4 value);
};