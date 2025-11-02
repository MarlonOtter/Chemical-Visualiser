#include "View2DLayer.h"

#include "View3DLayer.h"
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
				Vector2 offset = Perpendicular * ((m_BondSeperation * DefaultBondSeperation) * j - ((m_BondSeperation * DefaultBondSeperation) * (bondOrder - 1) / 2));

				Core::Shape::Line::DrawEx(
					(StartPos + offset) * m_WorldScale,
					(EndPos + offset) * m_WorldScale,
					m_BondWidth * DefaultBondWidth * static_cast<float>(m_WorldScale),
					Core::RAYWHITE
				);
			}
		}

		// ATOMS
		const float DefaultAtomSize = 0.25f;
		for (size_t i = 0; i < atoms.Types.size(); i++)
		{
			int posX = atoms.Positions2D.x[i] * m_WorldScale;
			int posY = atoms.Positions2D.y[i] * m_WorldScale;

			Core::Shape::Circle::Draw(
				posX, posY,
				m_AtomSize * DefaultAtomSize * static_cast<float>(m_WorldScale) * (atoms.Types[i] == 1 ? m_HydrogenScale : 1),
				ChemVis::GetAtomColor(atoms.Types[i]));
			if (m_ShowElementSymbol)
			{
				std::string Symbol = ChemVis::GetAtomSymbol(atoms.Types[i]);
				int FontSize = 0.2 * m_WorldScale;
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
	m_Camera.zoom = 100 / static_cast<float>(m_WorldScale);
	m_Camera.rotation = 0.0f;
	m_Camera.target = { 0,0 };
}