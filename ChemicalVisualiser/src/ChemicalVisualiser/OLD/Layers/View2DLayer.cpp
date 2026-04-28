#include "View2DLayer.h"

#include "View3DLayer.h"
#include "AppLayer.h"

View2DLayer::View2DLayer()
	: m_Target(800, 800)
{
	ResetCamera();
}

View2DLayer::View2DLayer(std::shared_ptr<ChemVis::Chemical> chem) : m_Chemical(chem), m_Target(800, 800)
{
	auto& values = Slate::Application::Get().Layers().Get<AppLayer>()->GetSettings().Values();

	auto& positions = chem.get()->GetAtoms().Positions2D;
	glm::vec2 center{ 0,0 };
	{
		//Core::Math::Mean(positions.x) * values.WorldScale2D,
		//Core::Math::Mean(positions.y)* values.WorldScale2D
	};

	ResetCamera(center);
}

View2DLayer::~View2DLayer()
{	
}

void View2DLayer::OnUpdate(float ts)
{
	if (m_WindowData.closed) return;

	Slate::Application& app = Slate::Application::Get();
	Slate::InputsHandler& Inputs = app.Inputs();
	glm::vec2 windowSize = app.Windows().GetMain()->GetSize();

	if (m_ResizeQueued)
	{
		SetupRenderTexture();
		m_PrevSize = { static_cast<float>(m_WindowData.width), static_cast<float>(m_WindowData.height) };
		m_Camera.offset = { static_cast<float>(m_WindowData.width) / 2, static_cast<float>(m_WindowData.height) / 2 };
		m_ResizeQueued = false;
	}

	if (Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT) && ((m_WindowData.width != static_cast<int>(m_PrevSize.x)) || (m_WindowData.height != static_cast<int>(m_PrevSize.y))))
	{
		m_Resizing = true;
	}
	// Should this be in an event?
	if ((m_Resizing && !Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT)) || IsWindowResized() || m_WindowData.dockChange || m_FirstFrame)
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
	// Just use OnEventRender instead of all this
	if ((m_WindowData.closed || (!m_WindowData.focused && !m_WindowData.hovered)) && !m_ForceRender) return;
	m_ForceRender = false;

	auto& values = Slate::Application::Get().Layers().Get<AppLayer>()->GetSettings().Values();

	m_Target.Activate();
	m_Target.Clear(glm::vec4(values.BackgroundColor2D[0], values.BackgroundColor2D[1], values.BackgroundColor2D[2], values.Background2D ? 255 : 0));

	//TODO This can be statically batched instead
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

			glm::vec2 StartPos = { atoms.Positions2D.x[startIndex], atoms.Positions2D.y[startIndex] };
			glm::vec2 EndPos = { atoms.Positions2D.x[endIndex], atoms.Positions2D.y[endIndex] };			
			glm::vec2 Direction = EndPos - StartPos;
			glm::vec2 Perpendicular = glm::normalize(glm::vec2{ Direction.y, -Direction.x });
			
			for (int j = 0; j < bondOrder; j++)
			{
				glm::vec2 offset = Perpendicular * ((values.BondSeperation2D * DefaultBondSeperation) * j - ((values.BondSeperation2D * DefaultBondSeperation) * (bondOrder - 1) / 2));

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

	m_Target.Deactivate(800, 800); // TODO REPLACE WITH ACTUAL VALUES 
}


void View2DLayer::HandleCameraMovement(float ts, glm::vec2 windowSize)
{
	auto& values = Slate::Application::Get().GetLayer<AppLayer>()->GetSettings().Values();
	m_Camera.target = glm::lerp(m_Camera.target, m_TargetPosition, 0.01f + ts * 30.0f * (1.0f-values.CameraSmoothing2D));

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		m_TargetPosition.x += -GetMouseDelta().x / m_Camera.zoom;
		m_TargetPosition.y += -GetMouseDelta().y / m_Camera.zoom;
	}

	m_Camera.zoom = glm::lerp(m_Camera.zoom, m_CameraZoom, 0.01f + ts * 7.5f * (1.0f-values.CameraSmoothing2D));

	float scroll = std::clamp(GetMouseWheelMove(), -1.0f, 1.0f) * 0.1f + 1.0f;
	m_CameraZoom *= scroll;
}

void View2DLayer::SetupRenderTexture()
{

	int w = std::fmax(m_WindowData.width, 10);
	int h = std::fmax(m_WindowData.height, 10);
	m_Target = Slate::RenderTexture(w, h); //! I think this is an issue with Slate, a bit of an oversight.
	m_ForceRender = true;
}

void View2DLayer::ResetCamera(glm::vec2 Target)
{
	m_Camera = {};
	m_CameraZoom = 100 / static_cast<float>(Slate::Application::Get().Layers().Get()<AppLayer>()->GetSettings().Values().WorldScale2D);
	m_Camera.zoom = m_CameraZoom;
	m_Camera.rotation = 0.0f;

	m_Camera.target = Target;
	m_TargetPosition = Target;
}