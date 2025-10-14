#include <Core/Application.h>

#include "Layers/AppLayer.h"
#include "Layers/View2DLayer.h"
//#include "Layers/View3DLayer.h"


int main()
{   
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "My Core Application";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.IsResizeable = true;
	appSpec.WindowSpec.VSync = true;

	Core::Application app(appSpec);

	// application logic: fetch data, etc.
	app.PushLayer<AppLayer>();
	
	// The structure rendering
	app.PushLayer<View2DLayer>();
	//app.PushLayer<View3DLayer>();
	
	app.Run();
}