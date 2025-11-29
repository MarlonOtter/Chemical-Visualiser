#include "Exporter.h"

#include "Layers/View2DLayer.h"
#include "Layers/View3DLayer.h"
#include "Layers/AppLayer.h"


namespace ChemVis
{
	Exporter::Exporter()
	{

	}

	Exporter::~Exporter()
	{

	}

	void Exporter::Export(std::string dir)
	{
		switch (m_Config.Mode)
		{
			case (ExportMode::Image) :
			{
				ExportImage(dir);
				break;
			}
			case (ExportMode::Model) :
			{
				ExportModel(dir);
				break;
			}
			case (ExportMode::Mol) :
			{
				ExportMol(dir);
				break;
			}
		}
	}

	void Exporter::ExportImage(std::string dir)
	{
		Core::Application& application = Core::Application::Get();

		RenderTexture2D RenderTex;
		if (m_Config.Visualiser == Visualiser2D)
		{
			View2DLayer* View2D = application.GetLayer<View2DLayer>();
			//TODO : Set Background to the desired value then re-render  
			RenderTex = View2D->getRenderTexture();
		}
		else if (m_Config.Visualiser == Visualiser3D)
		{
			View3DLayer* View3D = application.GetLayer<View3DLayer>();
			//TODO : Set Background to the desired value then re-render  
			RenderTex = View3D->getRenderTexture();
		}
		else {
			std::cout << "RENDER TEXTURE NOT LOADED\n";
			return;
		}
		::Image image = LoadImageFromTexture(RenderTex.texture);
		::ExportImage(image, (dir + ".png").c_str());

		::UnloadImage(image);
	}

	void Exporter::ExportModel(std::string dir)
	{
		std::cout << "CANNOT EXPORT MODEL : NOT YET IMPLEMENTED\n";
	}

	void Exporter::ExportMol(std::string dir)
	{
		std::cout << "CANNOT EXPORT .MOL : NOT YET IMPLEMENTED\n";
	}
}