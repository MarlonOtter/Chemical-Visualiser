#include "View3DLayer.h"

#include "View2DLayer.h"
#include "AppLayer.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Text.h"

View3DLayer::View3DLayer() {
	ResetCamera();
}

View3DLayer::View3DLayer(std::shared_ptr<ChemVis::Chemical> chem) : m_Chemical(chem)
{
	ResetCamera();
}

View3DLayer::~View3DLayer()
{
	UnloadRenderTexture(m_Target);
}

void View3DLayer::Update(float ts)
{
	if (m_WindowData.closed) return;

	Core::Application& app = Core::Application::Get();
	Vector2 windowSize = app.GetWindowSize();

	
	if (m_ResizeQueued)
	{
		UnloadRenderTexture(m_Target);
		SetupRenderTexture();
		m_PrevSize = { static_cast<float>(m_WindowData.width), static_cast<float>(m_WindowData.height) };
		m_ResizeQueued = false;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ((m_WindowData.width != static_cast<int>(m_PrevSize.x)) || (m_WindowData.height != static_cast<int>(m_PrevSize.y))))
	{
		m_Resizing = true;
	}
	if ((m_Resizing && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) || IsWindowResized() || m_WindowData.dockChange || m_FirstFrame)
	{
		m_FirstFrame = false;
		m_Resizing = false;
		m_ResizeQueued = true;
	}

	if (m_WindowData.focused && m_WindowData.hovered)
	{
		HandleCameraMovement(ts, windowSize);

		if (IsKeyPressed(KEY_L)) {
			m_DebugCamera = !m_DebugCamera;
		}
	}
}

void View3DLayer::OnRender()
{
	if ((m_WindowData.closed || (!m_WindowData.focused && !m_WindowData.hovered)) && !m_ForceRender) return;
	m_ForceRender = false;

	auto values = Core::Application::Get().GetLayer<AppLayer>()->GetSettings().Values();
	m_Camera.LookSensitivity() = values.LookSensitivity3D;
	m_Camera.PanSensitivity() = values.PanSensitivity3D;

	BeginTextureMode(m_Target);
	ClearBackground(m_ClearColor);
	BeginMode3D(m_Camera.GetHandler());

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

				Vector3 StartPos = { atoms.Positions3D.x[startIndex], atoms.Positions3D.y[startIndex], atoms.Positions3D.z[startIndex] };
				Vector3 EndPos = { atoms.Positions3D.x[endIndex], atoms.Positions3D.y[endIndex], atoms.Positions3D.z[endIndex] };
				Vector3 Direction = EndPos - StartPos;
				Vector3 Perpendicular = Vector3Normalize(Vector3CrossProduct(Direction, { 0,0,1 }));

				for (size_t j = 0; j < bondOrder; j++)
				{
					Vector3 offset = Perpendicular * ((values.BondSeperation3D * DefaultBondSeperation) * j - ((values.BondSeperation3D * DefaultBondSeperation) * (bondOrder - 1) / 2));

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
	
	EndMode3D();
	if (!m_Chemical || m_Chemical->GetAtoms().Positions3D.x.empty())
	{
		std::string text = "No 3D Position Data Available";
		int textWidth = Core::Text::Measure(text.c_str(), 30);
		Core::Text::Draw(text.c_str(), (m_WindowData.width - textWidth) / 2, m_WindowData.height / 2, 30, Core::WHITE);
	}

	EndTextureMode();
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
	m_Target = LoadRenderTexture(w, h);
	m_ForceRender = true;
}

void View3DLayer::ResetCamera()
{
	m_Camera.Update(0.0f, m_WindowData.width, m_WindowData.height);
}