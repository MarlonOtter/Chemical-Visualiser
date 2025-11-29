#pragma once

#include <string>

namespace ChemVis
{
	enum Visualiser {
		Visualiser2D,
		Visualiser3D
	};
	enum ExportMode {
		Image,
		Model,
		Mol
	};
	enum ImageExportType {
		PNG,
		JPG,
		BMP,
		TGA
	};
	enum ModelExportType {
		OBJ
	};

	struct ExportConfig
	{
		Visualiser Visualiser;
		ExportMode Mode;
	};

	struct ExportImageConfig
	{
		int Size[2] = {100, 100};
		bool Background = true;
		ImageExportType type = PNG;
	};

	struct ExportModelConfig
	{
		float Scale = 1.0f;
		int Quality = 0;
		ModelExportType type = OBJ;
	};

	class Exporter
	{
	public:
		Exporter();
		~Exporter();

		ExportConfig* Config() { return &m_Config; }
		ExportImageConfig* ImageConfig() { return &m_ImageConfig; }
		ExportModelConfig* ModelConfig() { return &m_ModelConfig; }
		
		void Export(std::string dir);


		void ExportImage(std::string dir);
		void ExportModel(std::string dir);
		void ExportMol(std::string dir);

		ExportConfig m_Config;
		ExportImageConfig m_ImageConfig;
		ExportModelConfig m_ModelConfig;
	};
}