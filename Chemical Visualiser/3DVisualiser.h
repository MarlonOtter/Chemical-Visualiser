#pragma once

#include "Viewport.h"
#include "ArcBallCamera.h"
#include "ChemicalVis/DrawChemical.h"


#include "ChemicalVis/Settings.h"

class Visualiser3D
{
public:
	Visualiser3D(float& screenRatio, ImGuiIO& io);

	ArcBallCamera* camera;
	Viewport* viewport;
	
	void Update(float windowWidth, float windowHeight);

	void Draw(std::vector<Chemical> chemicals);

private:
	float cameraFOV = 60.0f;
	float cameraNear = 0.1f;
	float cameraFar = 300.0f;

	ImGuiIO* io;
	float* screenRatio;

	glm::vec3 bgColour;
};