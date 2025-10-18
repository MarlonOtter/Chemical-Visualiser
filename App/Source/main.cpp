#include <Core/Application.h>

#include "Layers/AppLayer.h"
#include "Layers/View2DLayer.h"
#include "Layers/View3DLayer.h"
#include "Layers/InterfaceLayer.h"

int main()
{   
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Chemical Visualiser";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.IsResizeable = true;
	appSpec.WindowSpec.VSync = true;

	Core::Application app(appSpec);

	app.PushLayer<AppLayer>();

	app.PushLayer<View2DLayer>();
	app.PushLayer<View3DLayer>();

	app.PushLayer<InterfaceLayer>();


	app.Run();
}