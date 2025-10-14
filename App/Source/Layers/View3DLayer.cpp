#include "View3DLayer.h"

#include "View2DLayer.h"


View3DLayer::View3DLayer() {
	SetupRenderTexture();
	ResetCamera();
}


View3DLayer::View3DLayer(ChemVis::Chemical& chem)
{
	SetupRenderTexture();
	ResetCamera();

	// chem to be drawn/displayed
}

View3DLayer::~View3DLayer()
{
	UnloadRenderTexture(target);
}

void View3DLayer::Update(float ts)
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	if (IsWindowResized())
	{
		std::cout << "resized\n";
		// resize render texture
		UnloadRenderTexture(target);
		target = LoadRenderTexture(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	}

	if (IsKeyPressed(KEY_ONE)) {
		TransitionTo<View2DLayer>();
		return;
	}

	// move to dubug UI
	if (IsKeyPressed(KEY_L)) {
		DebugCamera = !DebugCamera;
	}

	HandleCameraMovement(ts, windowSize);
}

void View3DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode3D(camera);

	DrawSphere({ 0,0,0 }, 1.0f, RED);
	
	EndMode3D();
	EndTextureMode();
}

void View3DLayer::OnComposite()
{
	// this will be in a imgui window later
	DrawTextureRec(target.texture, { 0, 0, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) }, { 0, 0 }, WHITE);
}


void View3DLayer::HandleCameraMovement(float ts, Vector2 windowSize)
{
	if (DebugCamera) {
		UpdateCamera(&camera, CAMERA_FREE);
		return;
	}
	
	// custom-orbit style camera
}

void View3DLayer::SetupRenderTexture()
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();
	target = LoadRenderTexture(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

}

void View3DLayer::ResetCamera()
{
	camera = {};

	camera.target = { 0,0,0 };
	camera.position = { 5, 0, 0 };
	camera.fovy = 60.0f;
	camera.up = { 0,1,0 };
	camera.projection = CAMERA_PERSPECTIVE;
}