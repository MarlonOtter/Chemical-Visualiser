#include "View2DLayer.h"

#include "View3DLayer.h"
#include "Core/Renderer/Text.h"
#include "Core/Renderer/Shape.h"

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

	if (viewportSize != prevSize)
	{
		prevSize = viewportSize;
		// resize render texture
		UnloadRenderTexture(target);
		target = LoadRenderTexture(static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));
	}

	if (IsKeyPressed(KEY_P))
	{
		HandleCameraMovement(ts, windowSize);
	}
}

void View2DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode2D(camera);

	std::string chemName = "N/a";
	if (chemical) {
		chemName = chemical->GetInfo().name; // ERROR

		ChemVis::AtomsInfo atoms = chemical->GetAtoms();

		for (size_t i = 0; i < atoms.Types.size(); i++)
		{
			Core::Shape::Circle::Draw(atoms.Positions2D.x[i] * 20.0f, atoms.Positions2D.y[i] * 20.0f, 2, BLUE);
		}
	}

	Core::Text::Draw(chemName.c_str(), 0, 50, 20, Core::Color{255,255,255});
	Core::Shape::Circle::Draw(0, 0, 5, RED); 

	EndMode2D();
	EndTextureMode();
}

void View2DLayer::OnComposite()
{
	// this will be in a imgui window later
	DrawTextureRec(target.texture, { 0, 0, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) }, { 0, 0 }, WHITE);
}


void View2DLayer::HandleCameraMovement(float ts, Vector2 windowSize)
{
	camera.offset = { windowSize.x / 2, windowSize.y / 2 };

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
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();
	target = LoadRenderTexture(static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));

}

void View2DLayer::ResetCamera()
{
	camera = {};
	camera.zoom = 10.0f;
	camera.rotation = 0.0f;
	camera.target = { 0,0 };
}