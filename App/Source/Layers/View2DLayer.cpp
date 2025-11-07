#include "View2DLayer.h"

#include "View3DLayer.h"
#include "AppLayer.h"
#include "Core/Renderer/Text.h"
#include "Core/Renderer/Shape.h"
#include "Core/Renderer/Model.h"

#include "raylib.h"

View2DLayer::View2DLayer()
{
	ResetCamera();
}


View2DLayer::View2DLayer(std::shared_ptr<ChemVis::Chemical> chem) : m_Chemical(chem)
{
	ResetCamera();
}

View2DLayer::~View2DLayer()
{	
	UnloadRenderTexture(m_Target);
}

void View2DLayer::Update(float ts)
{
	if (m_WindowData.closed) return;

	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	 
	if (m_ResizeQueued)
	{
		UnloadRenderTexture(m_Target);
		SetupRenderTexture();
		m_PrevSize = { static_cast<float>(m_WindowData.width), static_cast<float>(m_WindowData.height) };
		m_Camera.offset = { static_cast<float>(m_WindowData.width) / 2, static_cast<float>(m_WindowData.height) / 2 };
		m_ResizeQueued = false;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ((m_WindowData.width != static_cast<int>(m_PrevSize.x)) || (m_WindowData.height != static_cast<int>(m_PrevSize.y))))
	{
		m_Resizing = true;
	}
	if ((m_Resizing && IsMouseButtonUp(MOUSE_BUTTON_LEFT)) || IsWindowResized() || m_WindowData.dockChange || m_FirstFrame)
	{
		m_FirstFrame = false;
		m_Resizing = false;
		m_ResizeQueued = true;		
	}
		
	if (m_WindowData.focused && m_WindowData.hovered)
	{
		HandleCameraMovement(ts, windowSize);
	}
}

void View2DLayer::OnRender()
{
	if (m_WindowData.closed) return;

	auto& values = Core::Application::Get().GetLayer<AppLayer>()->GetSettings().Values();

	BeginTextureMode(m_Target);
	ClearBackground(m_ClearColor);
	BeginMode2D(m_Camera);

	if (m_Chemical) {
		ChemVis::AtomsInfo atoms = m_Chemical->GetAtoms();
		ChemVis::BondsInfo bonds = m_Chemical->GetBonds();
		
		// BONDS
		const float DefaultBondWidth = 0.1f;
		const float DefaultBondSeperation = 0.2f;
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
				Vector2 offset = Perpendicular * ((values.BondSeperation2D * DefaultBondSeperation) * j - ((values.BondSeperation2D * DefaultBondSeperation) * (bondOrder - 1) / 2));

				Core::Shape::Line::DrawEx(
					(StartPos + offset) * values.WorldScale2D,
					(EndPos + offset) * values.WorldScale2D,
					values.BondWidth2D * DefaultBondWidth * static_cast<float>(values.WorldScale2D),
					Core::RAYWHITE
				);
			}
		}

		// ATOMS
		const float DefaultAtomScale = 0.25f;
		for (size_t i = 0; i < atoms.Types.size(); i++)
		{
			int posX = atoms.Positions2D.x[i] * values.WorldScale2D;
			int posY = atoms.Positions2D.y[i] * values.WorldScale2D;

			Core::Shape::Circle::Draw(
				posX, posY,
				values.AtomScale2D * DefaultAtomScale * static_cast<float>(values.WorldScale2D) * (atoms.Types[i] == 1 ? values.HydrogenScale2D : 1),
				ChemVis::Chemical::GetAtomColor(atoms.Types[i]));
			if (values.ShowElementLabels)
			{
				std::string Symbol = ChemVis::Chemical::GetAtomSymbol(atoms.Types[i]);
				int FontSize = values.LabelScale * values.WorldScale2D;
				Core::Text::Draw(Symbol.c_str(), posX-Core::Text::Measure(Symbol, FontSize)/2, posY-FontSize/2, FontSize, Core::BLACK);
			}
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
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		m_Camera.target.x += -GetMouseDelta().x / m_Camera.zoom;
		m_Camera.target.y += -GetMouseDelta().y / m_Camera.zoom;
	}

	float scroll = Clamp(GetMouseWheelMove(), -1.0f, 1.0f) * 0.1f + 1.0f;
	m_Camera.zoom *= scroll;
}

void View2DLayer::SetupRenderTexture()
{
	int w = std::fmax(m_WindowData.width, 10);
	int h = std::fmax(m_WindowData.height, 10);
	m_Target = LoadRenderTexture(w, h);
	SetTextureFilter(m_Target.texture, TEXTURE_FILTER_BILINEAR);
}

void View2DLayer::ResetCamera()
{
	m_Camera = {};
	m_Camera.zoom = 100 / static_cast<float>(Core::Application::Get().GetLayer<AppLayer>()->GetSettings().Values().WorldScale2D);
	m_Camera.rotation = 0.0f;
	m_Camera.target = { 0,0 };
}