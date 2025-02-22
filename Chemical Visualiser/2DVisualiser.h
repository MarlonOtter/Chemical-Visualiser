#pragma once

#include "Viewport.h"
#include "Camera2D.h"
#include "ChemicalVis/DrawChemical.h"

#include "ChemicalVis/Settings.h"

class Visualiser2D
{
public:
	Visualiser2D(float& screenRatio, ImGuiIO& io, Model& atomModel, Model& bondModel, Shader& atomShader, Shader& bondShader);

	Camera2D* camera;
	Viewport* viewport;

	void Update(float windowWidth, float windowHeight);

	void Draw(std::vector<Chemical> chemicals);

private:
	Model* atomModel;
	Model* bondModel;
	Shader* atomShader;
	Shader* bondShader;

	ImGuiIO* io;
	glm::vec3 bgColour = glm::vec3(0.85, 0.85, 0.85);
	float* screenRatio;

	float cameraNear;
	float cameraFar;
};