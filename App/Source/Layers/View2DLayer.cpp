#include "View2DLayer.h"

#include "View3DLayer.h"

View2DLayer::View2DLayer()
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();
	target = LoadRenderTexture(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	camera.zoom = 10.0f;
	camera.rotation = 0.0f;
	camera.target = { 0,0 };
}

View2DLayer::~View2DLayer()
{
	UnloadRenderTexture(target);
}

void View2DLayer::Update(float ts)
{
	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	if (IsWindowResized())
	{
		// resize render texture
		UnloadRenderTexture(target);
		target = LoadRenderTexture(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	}

	if (IsKeyPressed(KEY_TWO)) {
		TransitionTo<View3DLayer>();
		return;
	}
	
	HandleCameraMovement(ts, windowSize);
}

void View2DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode2D(camera);

	DrawCircle(400, 400, 500, RED);
	
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