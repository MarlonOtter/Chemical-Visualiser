#pragma once
#include "ChemicalVis/Settings.h"
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include <iostream>

class SettingsUI
{
public:
	SettingsUI();

	void Draw();

	void Refresh();

private:
	float bondColour[3];
	std::vector<float*> elementColours;



	void drawElementList();
};