#include "View3DLayer.h"

#include "View2DLayer.h"
#include "AppLayer.h"

View3DLayer::View3DLayer() 
	: m_Target(800, 800)
{
	ResetCamera();
}

View3DLayer::View3DLayer(std::shared_ptr<ChemVis::Chemical> chem) 
	: m_Chemical(chem), m_Target(800, 800)
{
	auto& positions = chem.get()->GetAtoms().Positions3D;
	glm::vec3 center =
	{
		Core::Math::Mean(positions.x),
		Core::Math::Mean(positions.y),
		Core::Math::Mean(positions.z)
	};

	ResetCamera(center);
}

View3DLayer::~View3DLayer()
{
}

void View3DLayer::OnUpdate(float ts)
{
	if (m_WindowData.closed) return;

	Slate::Application& app = Slate::Application::Get();
	glm::vec2 windowSize = app.Windows().GetMain()->GetSize();
	Slate::InputsHandler& Inputs = app.Inputs();
	
	if (m_ResizeQueued)
	{
		SetupRenderTexture();
		m_PrevSize = { static_cast<float>(m_WindowData.width), static_cast<float>(m_WindowData.height) };
		m_ResizeQueued = false;
	}

	if (Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT) && ((m_WindowData.width != static_cast<int>(m_PrevSize.x)) || (m_WindowData.height != static_cast<int>(m_PrevSize.y))))
	{
		m_Resizing = true;
	}
	if ((m_Resizing && !Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT)) || IsWindowResized() || m_WindowData.dockChange || m_FirstFrame)
	{
		m_FirstFrame = false;
		m_Resizing = false;
		m_ResizeQueued = true;
	}

	if (m_WindowData.focused && m_WindowData.hovered)
	{
		HandleCameraMovement(ts, windowSize);

		if (Inputs.IsKeyPressed(Slate::Keycode::L)) {
			m_DebugCamera = !m_DebugCamera;
		}
	}
}

void View3DLayer::OnRender()
{
	if ((m_WindowData.closed || (!m_WindowData.focused && !m_WindowData.hovered)) && !m_ForceRender) return;
	m_ForceRender = false;

	auto values = Slate::Application::Get().Layers().Get<AppLayer>()->GetSettings().Values();
	m_Camera.LookSensitivity() = values.LookSensitivity3D;
	m_Camera.PanSensitivity() = values.PanSensitivity3D;

	m_Target.Activate();
	m_Target.Clear(glm::vec4(values.BackgroundColor3D[0], values.BackgroundColor3D[1], values.BackgroundColor3D[2], values.Background3D ? 255 : 0));


	// TODO Statically Batch this
	if (m_Chemical) {
		ChemVis::AtomsInfo atoms = m_Chemical->GetAtoms();
		
		if (!atoms.Positions3D.x.empty())
		{
			// ATOMS
			const float DefaultAtomSize = 0.35f;
			for (size_t i = 0; i < atoms.Types.size(); i++)
			{
				Core::Model::Sphere::Draw(
					atoms.Positions3D.x[i], atoms.Positions3D.y[i], atoms.Positions3D.z[i],
					values.AtomScale3D * DefaultAtomSize * (atoms.Types[i] == 1 ? values.HydrogenScale3D : 1),
					ChemVis::Chemical::GetAtomColor(atoms.Types[i]));
			}

			// BONDS
			const float DefaultBondRadius = 0.08f;
			const float DefaultBondSeperation = 0.25;
			const int DefaultBondDetail = 20;
			ChemVis::BondsInfo bonds = m_Chemical->GetBonds();
			for (size_t i = 0; i < bonds.BeginAtomIndices.size(); i++)
			{
				int startIndex = bonds.BeginAtomIndices[i] - 1;
				int endIndex = bonds.EndAtomIndices[i] - 1;
				int bondOrder = bonds.BondOrders[i];

				glm::vec3 StartPos = { atoms.Positions3D.x[startIndex], atoms.Positions3D.y[startIndex], atoms.Positions3D.z[startIndex] };
				glm::vec3 EndPos = { atoms.Positions3D.x[endIndex], atoms.Positions3D.y[endIndex], atoms.Positions3D.z[endIndex] };
				glm::vec3 Direction = EndPos - StartPos;
				glm::vec3 Perpendicular = glm::normalize(glm::cross(Direction, { 0,0,1 }));

				for (size_t j = 0; j < bondOrder; j++)
				{
					glm::vec3 offset = Perpendicular * ((values.BondSeperation3D * DefaultBondSeperation) * j - ((values.BondSeperation3D * DefaultBondSeperation) * (bondOrder - 1) / 2));

					Core::Model::Cylinder::DrawEx(
						StartPos + offset,
						EndPos + offset,
						values.BondRadius3D * DefaultBondRadius,
						values.BondRadius3D * DefaultBondRadius,
						static_cast<int>(values.BondDetail3D * DefaultBondDetail),
						Core::RAYWHITE
					);
				}
			}

		} 
	}
	
	//EndMode3D();
	
	if (!m_Chemical || m_Chemical->GetAtoms().Positions3D.x.empty())
	{
		std::string text = "No 3D Position Data Available";
		int textWidth = Core::Text::Measure(text.c_str(), 30);
		Core::Text::Draw(text.c_str(), (m_WindowData.width - textWidth) / 2, m_WindowData.height / 2, 30, Core::WHITE);
	}

	m_Target.Deactivate();
}

void View3DLayer::OnComposite()
{
}


void View3DLayer::HandleCameraMovement(float ts, Vector2 windowSize)
{
	if (m_DebugCamera) {
		UpdateCamera(&m_Camera.GetHandler(), CAMERA_FREE);
		return;
	}
	
	// custom-orbit style m_Camera
	m_Camera.Update(ts, m_WindowData.width, m_WindowData.height);
}

void View3DLayer::SetupRenderTexture()
{
	int w = std::fmax(m_WindowData.width, 10);
	int h = std::fmax(m_WindowData.height, 10);
	m_Target = Slate::RenderTexture(w, h);
	m_ForceRender = true;
}

void View3DLayer::ResetCamera(glm::vec3 Target)
{
	m_Camera.SetTarget(Target);
	m_Camera.Update(0.0f, m_WindowData.width, m_WindowData.height);
}