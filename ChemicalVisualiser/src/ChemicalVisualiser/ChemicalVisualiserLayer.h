#pragma once

#include <Slate/Core.h>
#include <Slate/Renderer.h>
#include <Slate/Assets.h>

#include "Chemical.h" // ?
#include "ChemicalAsset.h" //X
#include "ChemicalList.h" // ?

#include "Camera2D.h"
#include "ArcballCamera.h"

#include "Settings.h" // ?

namespace ChemicalVisualiser
{
	class ChemicalVisualiserLayer : public Slate::Layer
	{
	public:
		ChemicalVisualiserLayer();
		~ChemicalVisualiserLayer();

		void OnUpdate(float ts) override;
		void OnEvent(Slate::EventDispatcher& Dispatcher) override;
		void OnRender() override;	
		void OnImGuiRender() override;

		void SetChemical(const std::string& Name);
		Slate::AssetHandle<ChemicalAsset>& GetActiveChemicalAssetHandle();
		Slate::AssetHandle<ChemicalAsset>& GetInactiveChemicalAssetHandle();
	
	private:
		void OnChemicalLoaded(const Chemical& Chemical);

	public:
		ChemicalList ChemicalList;
		Settings Settings;

	private:
		std::array<Slate::AssetHandle<ChemicalAsset>, 2> m_Chemical;
		bool m_ActiveChemicalAsset = false;
		
		Slate::Mesh m_Quad;
		Slate::Mesh m_Sphere;
		Slate::Mesh m_Cylinder;

		Slate::Buffers m_Batch2D;
		Slate::Buffers m_Batch3D;

		Slate::Material m_Material2D;
		Slate::Material m_Material3D;

		Slate::RenderTexture m_Target2D;
		Slate::RenderTexture m_Target3D;

		Camera2D m_Camera2D;
		ArcballCamera m_Camera3D;

		glm::uvec2 m_NewSize2D = {0,0};
		glm::uvec2 m_NewSize3D = {0,0};
	};
}