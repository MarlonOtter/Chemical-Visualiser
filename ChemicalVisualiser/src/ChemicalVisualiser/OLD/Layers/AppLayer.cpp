#include "AppLayer.h"

#include "ChemVis/Chemical.h"

#include "View2DLayer.h"
#include "View3DLayer.h"
#include "InterfaceLayer.h"


AppLayer::AppLayer()
{
	m_FetchThread = std::make_unique<ChemVis::FetchThread>();
	SetChemical("Aspirin");
}

AppLayer::~AppLayer()
{
	if (m_AutoCompleteRequestActive)
	{
		m_AutoCompleteFuture.wait();
	}
}

void AppLayer::OnUpdate(float ts)
{
	if (m_DeleteAllCachedChemicals)
	{
		m_FetchThread.get()->RequestDeleteCacheAll();
		m_DeleteAllCachedChemicals = false;
	}
	if (m_DeleteQueue.size() > 0)
	{
		m_FetchThread.get()->RequestDeleteCache(m_DeleteQueue);
		m_DeleteQueue.clear();
	}

	if (m_Settings.isSaveQueued())
	{
		m_Settings.SaveToDisk();
	}
	if (m_Settings.isRevertQueued())
	{
		m_Settings.Revert();
	}

	m_CacheEmpty = m_FetchThread.get()->GetCachedChemicalCount() == 0;

	HandleChemicalStructure();
	HandleAutoComplete();
	
	ManageFramerate(ts);

	Slate::Application::Get().Layers().Get<View3DLayer>()->Camera().SetSmoothingRate(m_Settings.Values().CameraSmoothing3D);
}

void AppLayer::OnComposite()
{
	Slate::LayerStack& Layers = Slate::Application::Get().Layers();
	auto& rt2D = Layers.Get<View2DLayer>()->getRenderTexture();
	auto& rt3D = Layers.Get<View3DLayer>()->getRenderTexture();

	Layers.Get<InterfaceLayer>()->setRenderTextures(rt2D, rt3D);
}

void AppLayer::OnEvent(Slate::EventDispatcher& Dispatcher)
{
	
}

void AppLayer::UpdateCacheSnapshot()
{
	m_CacheSnapshot = m_FetchThread.get()->GetCachedListSnapshot();
}

void AppLayer::QueueDeleteCachedChemical(int cid)
{
	m_DeleteQueue.push_back(cid);
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
	auto& Layers = Slate::Application::Get().Layers();
	Layers.Get<View2DLayer>()->TransitionTo<View2DLayer>(chem);
	Layers.Get<View3DLayer>()->TransitionTo<View3DLayer>(chem);
	Layers.Get<InterfaceLayer>()->SetChemicalInfo(chem->GetInfo());
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
		Slate::Application::Get().Layers().Get<InterfaceLayer>()->SetAutoComplete(options);
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