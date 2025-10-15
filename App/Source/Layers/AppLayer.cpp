#include "AppLayer.h"

#include "Core/Application.h"

#include "View2DLayer.h"
#include "View3DLayer.h"
#include "InterfaceLayer.h"

#include "ChemVis/Chemical.h"

#include "Core/Http/HttpClient.h"
#include "Core/Renderer/Text.h"

AppLayer::AppLayer()
{
}

AppLayer::~AppLayer()
{
}

void AppLayer::Update(float ts)
{
	textX += 100.0f * ts;
	if (textX > 800.0f)
		textX = -200.0f;

	if (IsKeyPressed(KEY_M)) {
		DisplayChemicalStructure();
	}
}

void AppLayer::OnComposite()
{
	Core::Application& app = Core::Application::Get();
	auto& rt2D = app.GetLayer<View2DLayer>()->getRenderTexture();
	auto& rt3D = app.GetLayer<View3DLayer>()->getRenderTexture();

	app.GetLayer<InterfaceLayer>()->setRenderTextures(rt2D, rt3D);
}

void AppLayer::OnEvent(Core::Event& event)
{
	
}

void AppLayer::DisplayChemicalStructure()
{
	// if the user inputs chemical name

	//TODO also include the 3D structure
	auto r2D = Core::Http::Client::Get("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/water/JSON?record_type=2d");
	auto r3D = Core::Http::Client::Get("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/water/JSON?record_type=3d");


	auto chem = std::make_shared<ChemVis::Chemical>(ChemVis::Chemical::Parse(ChemVis::Merge2Dand3D(r2D.body, r3D.body)));

	Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>(chem);
	Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
}
 