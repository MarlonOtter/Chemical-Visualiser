#pragma once

#include <string>

namespace ChemVis
{
	enum Visualiser {
		Visualiser2D,
		Visualiser3D
	};
	enum ExportMode {
		Model,
		Image,
		Mol
	};

	struct ExportConfig
	{
		Visualiser Visualiser;
		ExportMode Mode;
	};

	struct ExportImageConfig
	{
		int ImageWidth = 0;
		int ImageHeight = 0;
		bool Background = true;
	};

	struct ExportModelConfig
	{
		float ModelScale = 1.0f;
		unsigned int ModelResolution = 0;
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


		void ExportImage();
		void ExportModel();
		void ExportMol();

		ExportConfig m_Config;
		ExportImageConfig m_ImageConfig;
		ExportModelConfig m_ModelConfig;
	};
}