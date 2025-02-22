#include "2DVisualiser.h"

Visualiser2D::Visualiser2D(float& screenRatio, ImGuiIO& io, Model& atomModel, Model& bondModel, Shader& atomShader, Shader& bondShader)
{
	Visualiser2D::io = &io;
	Visualiser2D::screenRatio = &screenRatio;

	Camera2D camera(800, 800, glm::vec3(8,2,10));
	Visualiser2D::camera = &camera;

	Viewport viewport(0, 0, 0, 0, false, true);
	Visualiser2D::viewport = &viewport;

	//Models
	Visualiser2D::atomModel = &atomModel;
	Visualiser2D::bondModel = &bondModel;

	//Shaders
	Visualiser2D::atomShader = &atomShader;
	Visualiser2D::bondShader = &bondShader;
}


void Visualiser2D::Update(float windowWidth, float windowHeight)
{
	Viewport viewport = *Visualiser2D::viewport;
	viewport.setPos(*screenRatio * windowWidth, 0);

	float width = viewport.getSize().x;
	viewport.setSize(glm::vec2(1.0f - (*screenRatio), 1.0f), glm::vec2(windowHeight, windowHeight));

	if (Settings::changed)
	{
		bgColour = std::get<glm::vec3>(Settings::GetValue("2D Visualiser background Colour").data);
		
	}
	viewport.Update(bgColour);

	viewport.AttachWindow(std::string("2D Visualiser"));

	// If the viewport is being hovered over
	if (viewport.getHovered())
	{
		// Do mouse Inputs 
		camera->MouseInputs(*io);
	}
	// If the viewport is focussed (clicked on)
	if (viewport.getFocussed())
	{
		// Do Keyboard Inputs
		camera->KeyInputs(*io);
	}
	//update the size of the window in the camera class
	camera->UpdateSize(viewport.getPos(), viewport.getSize());

	//update any matrices for the camera
	camera->UpdateMatrix(cameraNear, cameraFar);
}

void Visualiser2D::Draw(std::vector<Chemical> chemicals)
{
	DrawChemical::Draw(globalClass::chemicals, *atomModel, *bondModel, *camera, *atomShader, *bondShader, Chemical::_2D);
}