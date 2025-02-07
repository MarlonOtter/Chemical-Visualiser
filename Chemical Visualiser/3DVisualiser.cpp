#include "3DVisualiser.h"



Visualiser3D::Visualiser3D()
{
	ArcBallCamera camera(800, 800, glm::vec3(0, 0, -10));
	Visualiser3D::camera = &camera;

	Viewport viewport(0, 0, 0, 0, false, true);
	Visualiser3D::viewport = &viewport;
}

void Visualiser3D::Update()
{
	viewport->setPos(0, 0);
	//viewport->setSize(viewportRatio, glm::vec2(windowWidth, windowHeight));

	
	if (Settings::changed)
	{
		//bgColour = std::get<float*>(Settings::GetValue("3D Visualiser background Colour").data);
	}
	viewport->Update(bgColour);
}

void Visualiser3D::Draw(std::vector<Chemical> chemicals)
{

}