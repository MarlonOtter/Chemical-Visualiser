#ifndef GRID_CLASS_H
#define GRID_CLASS_H

#include "DebugLine.h"

class Grid
{
	public:
		int size;
		float scale;

		DebugLine gridLine;

		DebugLine axisLine;
		

		Grid(int size, float scale = 1.0f);

		void Draw(Shader& gridShader, Shader& axisShader, Camera& camera);

	private:
		void CalculateGrid();
};

#endif // GRID_CLASS_H