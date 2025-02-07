#pragma once

#include "Viewport.h"
#include "ArcBallCamera.h"
#include "ChemicalVis/DrawChemical.h"

#include "ChemicalVis/Settings.h"

class Visualiser3D
{
public:
	Visualiser3D();

	ArcBallCamera* camera;
	Viewport* viewport;
	
	void Update();

	void Draw(std::vector<Chemical> chemicals);

private:
	float* bgColour;
};