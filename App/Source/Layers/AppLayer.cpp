#include "AppLayer.h"

#include "Core/Application.h"

#include "View2DLayer.h"
#include "View3DLayer.h"
#include "InterfaceLayer.h"

#include "ChemVis/Chemical.h"

#include "Core/Http/HttpClient.h"
#include "Core/Renderer/Text.h"
#include "Core/Utils/String.h"

#include "Core/Utils/Inputs.h"

AppLayer::AppLayer()
{
	chemical = "caffeine";
}

AppLayer::~AppLayer()
{
}

void AppLayer::Update(float ts)
{
	static bool startup = true;
	if (chemicalRecieved || startup) {
		DisplayChemicalStructure(chemical);
		chemicalRecieved = false;
	}
	startup = false;


	static int target = 60;
	static int maxFrameDelay = 10;
	static int frameDelay = maxFrameDelay;
	
	if (Core::anyInputs()) 
	{
		// reduce framerate after a short delay
		if (target != 10 && frameDelay <= 0) {
			frameDelay = maxFrameDelay;
			target = 10;
			SetTargetFPS(target);
		}
		frameDelay--;
	}
	else
	{
		// if any input go back up to 60FPS 
		frameDelay = maxFrameDelay;
		if (target != 60) {
			target = 60;
			SetTargetFPS(target);
		}
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

void AppLayer::SetChemical(std::string _chemical)
{
	chemical = _chemical;
	chemicalRecieved = true;
}

void AppLayer::DisplayChemicalStructure(std::string name)
{
	name = Core::String::Replace(name, ' ', "%20");

	Core::Http::HttpResponse r2D = Core::Http::Client::Get("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/"+name+"/JSON?record_type=2d");
	Core::Http::HttpResponse r3D = Core::Http::Client::Get("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/"+name+"/JSON?record_type=3d");

	auto chemObj = ChemVis::Chemical::Parse(ChemVis::Merge2Dand3D(r2D.body, r3D.body));
	if (!chemObj.has_value())
	{
		// send error to interface layer
		Core::Application::Get().GetLayer<InterfaceLayer>()->PushError("PubChem Request Failed");
		return;
	}

	auto chem = std::make_shared<ChemVis::Chemical>(chemObj.value());

	// regenerate the Layers with the new chemical
	Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>(chem);
	Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
}
 