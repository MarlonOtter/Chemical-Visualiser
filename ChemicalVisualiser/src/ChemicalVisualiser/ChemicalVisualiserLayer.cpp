#include "ChemicalVisualiserLayer.h"

#include <Slate/Events.h>
#include <Slate/Renderer.h>

#include <imgui.h>
#include <IconsFontAwesome6.h>

#include "ChemicalSource.h"

#include <Slate/MeshFactory.h>

namespace ChemicalVisualiser
{
	ChemicalVisualiserLayer::ChemicalVisualiserLayer()
		: m_Target2D(800, 800), m_Target3D(800, 800), m_Chemical({}), ChemicalList("Cache/Chemicals")
	{
		Slate::AssetManager& Assets = Slate::Application::Get().Assets();

		m_Quad = Slate::QuadMesh();
		m_Sphere = Slate::Meshes::GenerateIcoSphere(16, 1.0f);
		m_Cylinder = Slate::Meshes::GenerateCylinder(16, 1.0f, 1.0f, false);
	}

	ChemicalVisualiserLayer::~ChemicalVisualiserLayer()
	{
		
	}

	void ChemicalVisualiserLayer::OnEvent(Slate::EventDispatcher& Dispatcher)
	{
		Dispatcher.Subscribe<Slate::AssetLoadedEvent>([this](Slate::AssetLoadedEvent& e) 
			{
				auto& InactiveAssetHandle = GetInactiveChemicalAssetHandle();
				if (e.AssetId == InactiveAssetHandle.Id)
				{
					OnChemicalLoaded(InactiveAssetHandle.Get()->Chemical);
				}
				return false;
			});

		Dispatcher.Subscribe<Slate::MouseButtonReleasedEvent>([this](Slate::MouseButtonReleasedEvent& e)
			{
				// If Either have changed size
				// Resize render targets.
				if (m_NewSize2D.x)
				{
					m_Target2D.Resize(m_NewSize2D);
					m_Camera2D.Size = m_NewSize2D;
				}

				if (m_NewSize3D.x)
				{
				
					m_Target3D.Resize(m_NewSize3D);
					m_Camera3D.Size = m_NewSize3D;
				}
				return false;
			});
	}
	
	void ChemicalVisualiserLayer::OnChemicalLoaded(const Chemical& Chemical)
	{
		// Batch 2D
		std::vector<glm::mat4> Transforms(Chemical.Atoms.Count);
		for (const auto& Position : Chemical.Atoms.Position2D)
		{
			Transforms.push_back(glm::translate(glm::mat4(0.0f), glm::vec3(Position, 0.0f)));
		}
		m_Batch2D = Slate::BatchSingleMesh(m_Quad, Transforms);

		// Batch 3D
		std::vector<glm::mat4> Transforms(Chemical.Atoms.Count);
		for (const auto& Position : Chemical.Atoms.Position3D)
		{
			Transforms.push_back(glm::translate(glm::mat4(0.0f), Position));
		}
		m_Batch3D = Slate::BatchSingleMesh(m_Sphere, Transforms);

		// Unload the old Chemical and swap to the new one
		Slate::Application::Get().Assets().Unload(GetActiveChemicalAssetHandle());
		m_ActiveChemicalAsset = !m_ActiveChemicalAsset;
	}

	void ChemicalVisualiserLayer::OnUpdate(float ts)
	{
		// 2D
		m_Camera2D.Update(ts);

		// 3D
		m_Camera3D.Update(ts);
	}

	void ChemicalVisualiserLayer::OnRender()
	{
		m_Camera2D.CalculateMatrix();
		m_Target2D.Activate();
		Slate::DrawMesh(m_Batch2D, m_Material2D, &m_Camera2D);
		m_Target2D.Deactivate();

		m_Camera3D.CalculateMatrix();
		m_Target3D.Activate();
		Slate::DrawMesh(m_Batch3D, m_Material3D, &m_Camera3D);
		m_Target3D.Deactivate();
	}

	void ChemicalVisualiserLayer::OnImGuiRender()
	{
		if (ImGui::Begin(ICON_FA_SQUARE " 2D"))
		{
			ImVec2 WindowSize = ImGui::GetContentRegionMax();
			ImGui::Image(m_Target2D.Id, WindowSize);
		}
		ImGui::End();

		if (ImGui::Begin(ICON_FA_CUBE " 3D"))
		{
			ImVec2 WindowSize = ImGui::GetContentRegionMax();
			ImGui::Image(m_Target3D.Id, WindowSize);
		}
		ImGui::End();
	}

	void ChemicalVisualiserLayer::SetChemical(const std::string& Name)
	{
		// TODO Check that the inactive isn't mid load otherwise it would cause issues (memory leak).

		auto& InactiveChemicalHandle = GetInactiveChemicalAssetHandle();
		InactiveChemicalHandle = Slate::Application::Get().Assets().Load<ChemicalAsset>(std::make_unique<ChemicalNameSource>(Name, ChemicalList));
	}

	Slate::AssetHandle<ChemicalAsset>& ChemicalVisualiserLayer::GetActiveChemicalAssetHandle()
	{
		return m_Chemical[static_cast<size_t>(m_ActiveChemicalAsset)];
	}

	Slate::AssetHandle<ChemicalAsset>& ChemicalVisualiserLayer::GetInactiveChemicalAssetHandle()
	{
		return m_Chemical[static_cast<size_t>(!m_ActiveChemicalAsset)];
	}

}