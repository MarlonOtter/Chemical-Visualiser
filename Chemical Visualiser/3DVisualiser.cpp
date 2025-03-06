#include "3DVisualiser.h"



Visualiser3D::Visualiser3D(float& screenRatio, ImGuiIO& io)
{
	Visualiser3D::screenRatio = &screenRatio;
	Visualiser3D::io = &io;

	ArcBallCamera camera(800, 800, glm::vec3(0, 0, -10));
	Visualiser3D::camera = &camera;

	Viewport viewport(0, 0, 0, 0, false, true);
	Visualiser3D::viewport = &viewport;
}

void Visualiser3D::Update(float windowWidth, float windowHeight)
{
	viewport->setPos(0, 0);
	
	float width = viewport->getSize().x;
	viewport->setSize(glm::vec2(*screenRatio, 1.0f), glm::vec2(windowHeight, windowHeight));

	if (Settings::changed)
	{
		glm::vec3 col = std::get<glm::vec3>(Settings::GetValue("3D Visualiser background Colour").data);
		
	}

	viewport->Update(bgColour);

	viewport->AttachWindow("3D Visualiser");

	// If the viewport is being hovered over
	if (viewport->getHovered())
	{
		// Do mouse Inputs 
		camera->MouseInputs(*io);
	}
	// If the viewport is focussed (clicked on)
	if (viewport->getFocussed())
	{
		// Do Keyboard Inputs
		camera->KeyInputs(*io);
	}
	// Apply the rotation to the camera using the inputs
	camera->ApplyInputs();

	//update the size of the window in the camera class
	camera->camera.UpdateSize(viewport->getPos(), viewport->getSize());

	//update any matrices for the camera
	camera->UpdateMatrix(cameraFOV, cameraNear, cameraFar);
}
	

void Visualiser3D::Draw(std::vector<Chemical> chemicals)
{
	//DrawChemical::Draw(globalClass::chemicals, atomModel2D, bondModel3D, camera2D, atom2DShader, bondShader, Chemical::_2D);
}