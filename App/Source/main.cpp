#include <Core/Application.h>

#include "AppLayer.h"

int main()
{
    Core::PrintHello();
    
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "My Core Application";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.IsResizeable = true;
	
	Core::Application app(appSpec);
	app.PushLayer<AppLayer>();
	app.Run();
}