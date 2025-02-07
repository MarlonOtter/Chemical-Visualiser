#pragma once

#include "Viewport.h"
#include "Camera2D.h"
#include "ChemicalVis/DrawChemical.h"

#include "ChemicalVis/Settings.h"

class Visualiser2D
{
	Visualiser2D();

	Camera2D* camera;
	Viewport* viewport;

	void Update();

	void Draw(std::vector<Chemical> chemicals);
};