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
	if ((resizing && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) || IsWindowResized())
	{
		resizing = false;
		prevSize = { static_cast<float>(windowData.width), static_cast<float>(windowData.height) };
		// resize render texture
		UnloadRenderTexture(target);
		SetupRenderTexture();
	}

	// Toggle Debug Camera
	//? move to debug UI later
	if (IsKeyPressed(KEY_L)) {
		DebugCamera = !DebugCamera;
	}

	if (windowData.focused && windowData.hovered)
	{
		HandleCameraMovement(ts, windowSize);
	}
}

void View3DLayer::OnRender()
{
	BeginTextureMode(target);
	ClearBackground(clearColor);
	BeginMode3D(camera.GetHandler());

	if (chemical) {
		ChemVis::AtomsInfo atoms = chemical->GetAtoms();
		
		if (!atoms.Positions3D.x.empty())
		{
			// ATOMS
			for (size_t i = 0; i < atoms.Types.size(); i++)
			{
				Core::Model::Sphere::Draw(atoms.Positions3D.x[i], atoms.Positions3D.y[i], atoms.Positions3D.z[i], 0.25f, ChemVis::Chemical::GetColor(atoms.Types[i]));
			}

			// BONDS
			ChemVis::BondsInfo bonds = chemical->GetBonds();
			for (size_t i = 0; i < bonds.BeginAtomIndices.size(); i++)
			{
				int startIndex = bonds.BeginAtomIndices[i] - 1;
				int endIndex = bonds.EndAtomIndices[i] - 1;

				//? regenerates the mesh every frame would be better to generate the mesh and reuse it instead
				Core::Model::Cylinder::DrawEx(
					{ atoms.Positions3D.x[startIndex], atoms.Positions3D.y[startIndex], atoms.Positions3D.z[startIndex] },
					{ atoms.Positions3D.x[endIndex], atoms.Positions3D.y[endIndex], atoms.Positions3D.z[endIndex] },
					0.1, 0.1, 10, Core::GREEN
				);


				// implement this later
				for (size_t j = 0; j < bonds.BondOrders[i]; j++)
				{
					break;
				}
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
		UpdateCamera(&camera.GetHandler(), CAMERA_FREE);
		return;
	}
	
	// custom-orbit style camera
	camera.Update(ts);
}

void View3DLayer::SetupRenderTexture()
{
	target = LoadRenderTexture(windowData.width, windowData.height);
}

void View3DLayer::ResetCamera()
{
	camera.Update(0.0f);
}