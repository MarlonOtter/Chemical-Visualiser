#include "AppLayer.h"

AppLayer::AppLayer()
{
}

AppLayer::~AppLayer()
{
}

void AppLayer::Update(float ts)
{
	textX += 100.0f * ts;
	if (textX > 800.0f)
		textX = -200.0f;
}

void AppLayer::OnComposite()
{
	DrawText("Hello from AppLayer!", textX, 200, 20, RAYWHITE);
}
 