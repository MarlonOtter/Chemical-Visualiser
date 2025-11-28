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
				ExportImage();
			}
			case (ExportMode::Model) :
			{
				ExportModel();
			}
			case (ExportMode::Mol) :
			{
				ExportMol();
			}
		}
	}

	void Exporter::ExportImage()
	{
		Core::Application& application = Core::Application::Get();

		RenderTexture2D RenderTex;
		if (m_Config.Visualiser == Visualiser2D)
		{
			View2DLayer* View2D = application.GetLayer<View2DLayer>();
			RenderTex = View2D->getRenderTexture();
		}
		else if (m_Config.Visualiser == Visualiser3D)
		{
			View3DLayer* View3D = application.GetLayer<View3DLayer>();
			RenderTex = View3D->getRenderTexture();
		}
		else {
			std::cout << "RENDER TEXTURE NOT LOADED\n";
			return;
		}
		::Image image = LoadImageFromTexture(RenderTex.texture);
		::ExportImage(image, "ImageExport.png");

		::UnloadImage(image);
	}

	void Exporter::ExportModel()
	{

	}

	void Exporter::ExportMol()
	{

	}
}