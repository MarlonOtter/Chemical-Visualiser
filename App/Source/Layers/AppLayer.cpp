#include "AppLayer.h"

#include "Core/Application.h"
#include "Core/Http/HttpClient.h"
#include "Core/Utils/String.h"
#include "Core/Utils/Inputs.h"

#include "ChemVis/Chemical.h"

#include "View2DLayer.h"
#include "View3DLayer.h"
#include "InterfaceLayer.h"


AppLayer::AppLayer()
{
	m_FetchThread = std::make_unique<ChemVis::FetchThread>();
	SetChemical("Aspirin");

	// Preload element data (symbols and colours)
	ChemVis::Chemical::SetAtomColors(m_Settings.Values().ElementColors);
}

AppLayer::~AppLayer()
{
	if (m_AutoCompleteRequestActive)
	{
		m_AutoCompleteFuture.wait();
	}
}

void AppLayer::Update(float ts)
{
	if (m_DeleteCachedChemicals)
	{
		m_FetchThread.get()->RequestDeleteCache();
		m_DeleteCachedChemicals = false;
	}

	if (m_Settings.isSaveQueued())
	{
		m_Settings.SaveToDisk();
	}

	HandleChemicalStructure();
	HandleAutoComplete();
	
	ManageFramerate(ts);

	Core::Application::Get().GetLayer<View3DLayer>()->Camera().SetSmoothingRate(m_Settings.Values().CameraSmoothing3D);
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

void AppLayer::HandleChemicalStructure()
{
	// Request New Chemical
	if (m_ChemicalRecieved)
	{
		if (m_FetchThread && 
			// Check valid
			(m_CurrentlyDisplayed != m_Chemical) && !m_Chemical.empty()) 
		{
			m_FetchThread->RequestChemical(m_Chemical);
			m_CurrentlyDisplayed = m_Chemical;
		}
		m_ChemicalRecieved = false;
	}

	// Get Result from thread
	if (m_FetchThread.get()->IsResultReady())
	{
		auto chemical = m_FetchThread->GetResult();
		if (!chemical.GetAtoms().Types.empty())
		{
			std::cout << "Recieved chemical from thread\n";
			SendChemical(chemical);
		}
	}
}

void AppLayer::SendChemical(ChemVis::Chemical& chemical)
{
	auto chem = std::make_shared<ChemVis::Chemical>(chemical);
	Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>(chem);
	Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
	Core::Application::Get().GetLayer<InterfaceLayer>()->SetChemicalInfo(chem->GetInfo());
}
 
void AppLayer::HandleAutoComplete()
{
	if (!m_AutoCompleteInput.empty() && !m_AutoCompleteRequestActive)
	{
		m_AutoCompleteFuture = ChemVis::PubChem::Async::GetAutoComplete(m_AutoCompleteInput);
		m_AutoCompleteRequestActive = true;
		m_AutoCompleteInput.clear();
	}
	if (m_AutoCompleteRequestActive && ChemVis::PubChem::Async::isFutureReady(m_AutoCompleteFuture))
	{
		m_AutoCompleteRequestActive = false;
		auto options = m_AutoCompleteFuture.get();
		Core::Application::Get().GetLayer<InterfaceLayer>()->SetAutoComplete(options);
	}
}

void AppLayer::SetChemical(std::string chemical)
{
	m_Chemical = std::move(chemical);
	m_ChemicalRecieved = true;
}

void AppLayer::ManageFramerate(float ts)
{
	static bool targetMax = true;
	static int currentMaxFPS = m_Settings.Values().TargetFPS;

	const float maxFrameDelay = 0.5f;
	static float frameDelay = maxFrameDelay;

	const int maxTarget = m_Settings.Values().TargetFPS;
	const int minTarget = 10;

	if (currentMaxFPS != m_Settings.Values().TargetFPS)
	{
		currentMaxFPS = m_Settings.Values().TargetFPS;
		if (targetMax) {
			SetTargetFPS(currentMaxFPS);
		}
	}

	if (m_Settings.Values().DynamicFramerate == false)
	{
		targetMax = true;
		return;
	}

	if (Core::anyInputs())
	{
		// reduce framerate after a short delay
		if (targetMax && frameDelay <= 0) {
			frameDelay = maxFrameDelay;
			targetMax = false;
			SetTargetFPS(minTarget);
		}
		frameDelay -= ts;
	}
	else
	{
		// if any input go back up to max FPS 
		frameDelay = maxFrameDelay;
		if (!targetMax) {
			targetMax = true;
			SetTargetFPS(maxTarget);
			currentMaxFPS = maxTarget;
		}
	}
}