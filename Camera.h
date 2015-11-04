#pragma 
#include "FlyWin32.h"

class Camera
{
protected:
	OBJECTid parentObjectID;
	OBJECTid cameraID; 
	VIEWPORTid vID;

public:
	Camera();
	void LoadData(SCENEid sceneID);
	void Init(ROOMid terrainRoomID);
	void Update(int skip);
	void Render(int skip);
	void LookAt();
	~Camera();
};

