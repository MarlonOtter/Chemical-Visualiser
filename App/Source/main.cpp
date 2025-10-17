#include <Core/Application.h>

#include "Layers/AppLayer.h"
#include "Layers/View2DLayer.h"
#include "Layers/View3DLayer.h"
#include "Layers/InterfaceLayer.h"


int main()
{   
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "My Core Application";
	appSpec.WindowSpec.Width = 1200;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.IsResizeable = true;
	appSpec.WindowSpec.VSync = true;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	Core::Application app(appSpec);

	app.PushLayer<AppLayer>();
	app.PushLayer<View2DLayer>();
	
	app.PushLayer<View3DLayer>();
	app.PushLayer<InterfaceLayer>();


	app.Run();
}