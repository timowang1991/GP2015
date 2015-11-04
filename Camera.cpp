#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::LoadData(SCENEid sceneID)
{
	vID = FyCreateViewport(0, 0, 800, 600);
	FnViewport vp;
	vp.ID(vID);
	vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

	FnScene scene;
	scene.ID(sceneID);
	cameraID = scene.CreateObject(CAMERA);

	parentObjectID = scene.CreateObject(OBJECT);
	
	FnCamera camera;
	camera.ID(cameraID);
	camera.SetParent(parentObjectID);
}
void Camera::Init(ROOMid terrainRoomID)
{
	FnCamera camera;
	camera.ID(cameraID);
	camera.Rotate(X_AXIS, 90.0f, LOCAL);
	camera.Translate(0.0f, 70.0f, 400.0f, LOCAL);
}

void Camera::Update(int skip)
{
	/*FnCamera camera;
	camera.ID(cameraID);
	camera.Translate(0.0f, 70.0f, 400.0f, LOCAL);*/
}

void Camera::Render(int skip)
{

	FnViewport vp;

	// render the whole scene
	vp.ID(vID);
	vp.Render3D(cameraID, TRUE, TRUE);
}

void Camera::LookAt()
{}

