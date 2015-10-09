#include "FlyWin32.h"

class Player
{
private:
	int actorID;
	ACTIONid idleID, runID, curPoseID, nextPosID;
	
public:
	void LoadData(SCENEid sceneID);
	void Init(ROOMid terrainRoomID);
	void Update(int skip);
	void Render(int skip);
	void Input(BYTE code, BOOL4 value);
};