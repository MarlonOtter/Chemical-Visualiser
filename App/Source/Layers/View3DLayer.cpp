#include "View3DLayer.h"

#include "View2DLayer.h"
#include "Core/Renderer/Model.h"

View3DLayer::View3DLayer() {
	SetupRenderTexture();
	ResetCamera();
}


View3DLayer::View3DLayer(std::shared_ptr<ChemVis::Chemical> chem) : chemical(chem)
{
	SetupRenderTexture();
	ResetCamera();
}

View3DLayer::~View3DLayer()
{
	UnloadRenderTexture(target);
}

void View3DLayer::Update(float ts)
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ((windowData.width != static_cast<int>(prevSize.x)) || (windowData.height != static_cast<int>(prevSize.y))))
	{
		resizing = true;
	}
	else if (resizing && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		resizing = false;
		prevSize = { static_cast<float>(windowData.width), static_cast<float>(windowData.height) };
		// resize render texture
		UnloadRenderTexture(target);
		SetupRenderTexture();
	}

	// move to dubug UI
	if (IsKeyPressed(KEY_L)) {
		DebugCamera = !DebugCamera;
	}

	if (windowData.focused && windowData.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		HandleCameraMovement(ts, windowSize);
	}
}

void View3DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode3D(camera);
	
	//DrawSphere({ 0,0,0 }, 1, Core::BLUE);

	if (chemical) {
		ChemVis::AtomsInfo atoms = chemical->GetAtoms();
		if (!atoms.Positions3D.x.empty())
		{
			for (size_t i = 0; i < atoms.Types.size(); i++)
			{
				Core::Model::Sphere::Draw(atoms.Positions3D.x[i], atoms.Positions3D.y[i], atoms.Positions3D.z[i], 0.2f, BLUE);
			}
		}
	}
	
	EndMode3D();
	EndTextureMode();
}

void View3DLayer::OnComposite()
{
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
	target = LoadRenderTexture(windowData.width, windowData.height);
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