#include <Slate/Core.h>
#include <Slate/EntryPoint.h>

//#include "Layers/AppLayer.h"
//#include "Layers/InterfaceLayer.h"
//#include "Layers/View2DLayer.h"
//#include "Layers/View3DLayer.h"

#include "ChemicalVisualiser/ChemicalVisualiserLayer.h"
#include "ChemicalVisualiser/InterfaceLayer.h"

std::unique_ptr<Slate::Application> CreateApplication()
{
	//appSpec.WindowSpec.VSync = true;
	//appSpec.WindowSpec.IconPath = "icon.png";

	Slate::WindowSpecification WindowSpec{
		.Title = "Chemical Visualiser",
		.Width = 16 * 100,
		.Height = 9 * 100,
		.ClearColor = { 0, 0, 0, 255 },
		.Resizeable = true,
		.Borderless = false
	};

	Slate::ApplicationSpecification AppSpec{
		.Name = "Chemical Visualiser",
		.WindowSpec = WindowSpec,
		.FixedUpdatedFrequency = 20.0f,
		.EventRendering = false,
		.HotReloadAssets = true,
	};

	auto app = std::make_unique<Slate::Application>(AppSpec);

	//app->Layers().Push<AppLayer>();
	//app->Layers().Push<View2DLayer>();
	//app->Layers().Push<View3DLayer>();
	//app->Layers().Push<InterfaceLayer>();

	app->Layers().Push<ChemicalVisualiser::ChemicalVisualiserLayer>();
	app->Layers().Push<ChemicalVisualiser::InterfaceLayer>();


	return app;
}