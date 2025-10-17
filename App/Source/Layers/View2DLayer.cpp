#include "View2DLayer.h"

#include "View3DLayer.h"
#include "Core/Renderer/Text.h"
#include "Core/Renderer/Shape.h"
#include "Core/Renderer/Model.h"

#include "raylib.h"

View2DLayer::View2DLayer()
{
	SetupRenderTexture();
	ResetCamera();
}


View2DLayer::View2DLayer(std::shared_ptr<ChemVis::Chemical> chem) : chemical(chem)
{
	SetupRenderTexture();
	ResetCamera();
}

View2DLayer::~View2DLayer()
{	
	UnloadRenderTexture(target);
}

void View2DLayer::Update(float ts)
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	//std::cout << "width: " << windowData.width << ", height: " << windowData.height;

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ((windowData.width != static_cast<int>(prevSize.x)) || (windowData.height != static_cast<int>(prevSize.y))))
	{
		resizing = true;
	} 
	//! Window data is not updated when this is ran for the first frame
	//! so until the user interacts with the program it is pixelated. 
	if ((resizing && IsMouseButtonUp(MOUSE_BUTTON_LEFT)) || IsWindowResized())
	{
		resizing = false;
		prevSize = { static_cast<float>(windowData.width), static_cast<float>(windowData.height) };
		// resize render texture
		UnloadRenderTexture(target);
		SetupRenderTexture();
	}
		
	if (windowData.focused && windowData.hovered)
	{
		HandleCameraMovement(ts, windowSize);
	}
}

void View2DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode2D(camera);

	if (chemical) {
		ChemVis::AtomsInfo atoms = chemical->GetAtoms();
		ChemVis::BondsInfo bonds = chemical->GetBonds();
		for (size_t i = 0; i < bonds.BeginAtomIndices.size(); i++)
		{
			int startIndex = bonds.BeginAtomIndices[i] - 1;
			int endIndex = bonds.EndAtomIndices[i] - 1;

			Core::Shape::Line::DrawEx(
				{ atoms.Positions2D.x[startIndex] * 1000.0f , atoms.Positions2D.y[startIndex] * 1000.0f},
				{ atoms.Positions2D.x[endIndex] * 1000.0f, atoms.Positions2D.y[endIndex] * 1000.0f },
				50, 
				Core::RAYWHITE
				);
		}

		for (size_t i = 0; i < atoms.Types.size(); i++)
		{
			Core::Shape::Circle::Draw(atoms.Positions2D.x[i] * 1000.0f, atoms.Positions2D.y[i] * 1000.0f, 100, BLUE);
		}

	}

	EndMode2D();
	EndTextureMode();
}

void View2DLayer::OnComposite()
{
}


void View2DLayer::HandleCameraMovement(float ts, Vector2 windowSize)
{
	camera.offset = { static_cast<float>(windowData.width) / 2, static_cast<float>(windowData.height) / 2 };

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		camera.target.x += -GetMouseDelta().x / camera.zoom;
		camera.target.y += -GetMouseDelta().y / camera.zoom;
	}

	float scroll = Clamp(GetMouseWheelMove(), -1.0f, 1.0f) * 0.1f + 1.0f;
	camera.zoom *= scroll;
}

void View2DLayer::SetupRenderTexture()
{
	target = LoadRenderTexture(windowData.width, windowData.height);
}

void View2DLayer::ResetCamera()
{
	camera = {};
	camera.zoom = 0.1f;
	camera.rotation = 0.0f;
	camera.target = { 0,0 };
}