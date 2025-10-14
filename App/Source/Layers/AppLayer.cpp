#include "AppLayer.h"

#include "Core/Application.h"

#include "View2DLayer.h"
#include "View3DLayer.h"

#include "ChemVis/Chemical.h"

#include "Core/Http/HttpClient.h"

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
	DrawText("Hello from AppLayer!", textX, 200, 20, RAYWHITE);
}

void AppLayer::OnEvent(Core::Event& event)
{
	
}

void AppLayer::DisplayChemicalStructure()
{
	// if the user inputs chemical name

	//TODO also include the 3D structure
	auto r = Core::Http::Client::Get("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/water/JSON");


	auto chem = std::make_shared<ChemVis::Chemical>(ChemVis::Chemical::Parse(r.body));

	//! this don't work for some reason
	Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>(chem);
	//Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
}
 