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
	SetChemical("Caffeine");
}

AppLayer::~AppLayer()
{
	if (m_StructureRequestActive) 
	{
		m_StructureFuture.wait();
	}
	if (m_AutoCompleteRequestActive)
	{
		m_AutoCompleteFuture.wait();
	}
}

void AppLayer::Update(float ts)
{
	HandleChemicalStructure();
	HandleAutoComplete();

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

void AppLayer::HandleChemicalStructure()
{
	if (m_ChemicalRecieved && !m_StructureRequestActive)
	{
		m_StructureFuture = ChemVis::PubChem::Async::GetChemical(m_Chemical);
		m_StructureRequestActive = true;
		m_ChemicalRecieved = false;
	}
	if (m_StructureRequestActive && ChemVis::PubChem::Async::isFutureReady(m_StructureFuture))
	{
		m_StructureRequestActive = false;
		try {
			auto chemObj = m_StructureFuture.get();
			m_StructureRequestActive = false;

			if (!chemObj.GetAtoms().Types.empty())
			{
				auto chem = std::make_shared<ChemVis::Chemical>(chemObj);
				Core::Application::Get().GetLayer<View2DLayer>()->TransitionTo<View2DLayer>(chem);
				Core::Application::Get().GetLayer<View3DLayer>()->TransitionTo<View3DLayer>(chem);
				Core::Application::Get().GetLayer<InterfaceLayer>()->SetChemicalInfo(chem->GetInfo());
			}
		}
		catch (const std::exception& e) {
			m_StructureRequestActive = false;
			Core::Application::Get().GetLayer<InterfaceLayer>()->PushError(
				std::string("Chemical request failed: ") + e.what()
			);
		}
	}
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

void AppLayer::SetChemical(std::string _chemical)
{
	m_Chemical = _chemical;
	m_ChemicalRecieved = true;
}