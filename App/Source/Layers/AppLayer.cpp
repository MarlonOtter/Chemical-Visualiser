#include "AppLayer.h"

#include "Core/Application.h"

#include "View2DLayer.h"
#include "View3DLayer.h"

#include "ChemVis/Chemical.h"

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
}

void AppLayer::OnComposite()
{
	DrawText("Hello from AppLayer!", textX, 200, 20, RAYWHITE);
}

void AppLayer::OnEvent(Core::Event& event)
{
	// if the user inputs chemical name

	// fetch data from pubchem

	// on recieve event that the fetch is complete

	// parse the data into a chemical obj

	// push to View2DLayer and View3DLayer
	ChemVis::Chemical chem();

	//! this don't work for some reason
	Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>();
	//Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
}
 