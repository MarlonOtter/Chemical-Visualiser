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
	if (windowData.closed) return;

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
	if (windowData.closed) return;

	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode2D(camera);

	if (chemical) {
		ChemVis::AtomsInfo atoms = chemical->GetAtoms();
		ChemVis::BondsInfo bonds = chemical->GetBonds();
		// BONDS
		for (size_t i = 0; i < bonds.BeginAtomIndices.size(); i++)
		{
			int startIndex = bonds.BeginAtomIndices[i] - 1;
			int endIndex = bonds.EndAtomIndices[i] - 1;
			int bondOrder = bonds.BondOrders[i];

			Vector2 StartPos = { atoms.Positions2D.x[startIndex], atoms.Positions2D.y[startIndex] };
			Vector2 EndPos = { atoms.Positions2D.x[endIndex], atoms.Positions2D.y[endIndex] };			
			Vector2 Direction = EndPos - StartPos;
			Vector2 Perpendicular = Vector2Normalize({ Direction.y, -Direction.x });
			
			for (int j = 0; j < bondOrder; j++)
			{
				Vector2 offset = Perpendicular * (m_BondSeperation * j - (m_BondSeperation * (bondOrder - 1) / 2));

				Core::Shape::Line::DrawEx(
					(StartPos + offset) * m_WorldScale,
					(EndPos + offset) * m_WorldScale,
					m_BondWidth * static_cast<float>(m_WorldScale),
					Core::RAYWHITE
				);
			}
		}

		// ATOMS
		for (size_t i = 0; i < atoms.Types.size(); i++)
		{
			Core::Shape::Circle::Draw(
				atoms.Positions2D.x[i] * m_WorldScale, atoms.Positions2D.y[i] * m_WorldScale,
				m_AtomSize * static_cast<float>(m_WorldScale) * (atoms.Types[i] == 1 ? m_HydrogenScale : 1),
				ChemVis::Chemical::GetColor(atoms.Types[i]));
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
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
}

void View2DLayer::ResetCamera()
{
	camera = {};
	camera.zoom = 100 / static_cast<float>(m_WorldScale);
	camera.rotation = 0.0f;
	camera.target = { 0,0 };
}