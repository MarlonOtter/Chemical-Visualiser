#include "GlobalClass.h"

Camera* globalClass::camera2D;
Camera* globalClass::camera3D;

std::vector<Chemical> globalClass::chemicals;

Shader* globalClass::atomShader;
Model* globalClass::atomModel;

float globalClass::atomScale = 0.3;

float globalClass::bondRadius = 0.17f;
float globalClass::bondLengthMultiplier = 0.4f;
float globalClass::bondSeperationDist = 0.2f;

float globalClass::chemicalSeperationDist = 2.0f;
