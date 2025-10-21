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
	m_ChemicalList = ChemVis::ChemicalList("Cache/Chemicals");
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
		if (m_ChemicalList.IsStored(m_Chemical))
		{
			std::cout << "READING FROM CACHE\n";
			// Get data from file
			int cid = m_ChemicalList.GetCid(m_Chemical);
			std::cout << "CID: " << cid << "\n";
			std::string data = m_ChemicalList.GetData(cid);
			//std::cout << "DATA:\n" << data << "\n\n";
			auto chemical = ChemVis::Chemical::Parse(data);
			if (chemical.has_value())
			{
				SendChemical(chemical.value());
			}
		}
		else
		{
			// Get data from API
			std::cout << "USING API\n";
			m_StructureFuture = ChemVis::PubChem::Async::GetChemical(m_Chemical);
			m_StructureRequestActive = true;
		}

		m_ChemicalRecieved = false;
	}
	if (m_StructureRequestActive && ChemVis::PubChem::Async::isFutureReady(m_StructureFuture))
	{
		m_StructureRequestActive = false;
		try {
			auto result = m_StructureFuture.get();
			auto chemObj = result.Chemical;
			m_StructureRequestActive = false;

			//TODO STORE THE CHEMICAL IN m_ChemicalList.
			std::string ChemicalIdentifier = result.Identifier;
			if (!result.Data.empty() && !m_ChemicalList.IsStored(ChemicalIdentifier))
			{
				std::cout << "STORING IN CACHE\n";
				m_ChemicalList.Store(ChemicalIdentifier, std::stoi(chemObj.GetInfo().Cid), result.Data);
			}

			if (!chemObj.GetAtoms().Types.empty()) SendChemical(chemObj);
		}
		catch (const std::exception& e) {
			m_StructureRequestActive = false;
			Core::Application::Get().GetLayer<InterfaceLayer>()->PushError(
				std::string("Chemical request failed: ") + e.what()
			);
		}
	}
}

void AppLayer::SendChemical(ChemVis::Chemical& chemical)
{
	//! ERROR HERE: STRING TOO LONG
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

void AppLayer::SetChemical(std::string _chemical)
{
	m_Chemical = _chemical;
	m_ChemicalRecieved = true;
}