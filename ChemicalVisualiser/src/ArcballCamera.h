#pragma once

#include "Slate/Renderer.h"
#include <Slate/Math.h>

class ArcballCamera : public Slate::Camera 
{
public:
	ArcballCamera();
	void Update(float ts) override;

private:
	void CalculateRotation(float ts);
public:
	bool InvertX = false;
	bool InvertY = false;

private:
	glm::quat m_Rotation;
	float m_Smoothing = 0.0f;
	float m_LookSensitivity = 1.0f;
	glm::vec3 m_AngularVelocity = glm::vec3(0);
	float m_Distance = 1.0f;
	glm::vec3 m_TargetPanPosition = glm::vec3(0);
	float m_PanSensitivity = 1.0f;
	float m_TargetDistance = 1.0f;
};