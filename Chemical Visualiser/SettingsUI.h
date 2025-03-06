#pragma once
#include "ChemicalVis/Settings.h"
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <array>

class SettingsUI
{
public:
	SettingsUI();
	void Init();

	void Draw();

	void Refresh();

	void Save();

	

private:
	int NUM_ELEMENTS = 118;

	float* bondPointer;
	std::array<float, 3> bondColour = { 0, 0, 0 };


	std::array<std::array<float, 3>, 118> elementData;
	std::array<float*, 118> elementColours;

	void drawElementList();
	void ResetElements();

	static glm::vec3 toVec3(float*);
};