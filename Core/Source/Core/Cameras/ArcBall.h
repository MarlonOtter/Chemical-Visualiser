#pragma once

#include "raylib.h"

namespace Core::Camera
{
	class ArcBall
	{
	public:
		ArcBall(float Fov=60.0f);
		ArcBall(Vector3 Position, Vector3 Target, float Fov=60.0f);
		~ArcBall();

		Vector3 Position() const { return m_Handler.position; }
		Vector3& Target() { return m_Handler.target; }
		void SetTarget(Vector3 Target) { m_Handler.target = Target; }
		Camera3D& GetHandler() { return m_Handler; }

		void Update(float ts, int width, int height);
		
		void SetLookSensitivity(float Sensitivity) { m_LookSensitivity = Sensitivity; }
		void SetSmoothingRate(float newRate) { m_Smoothing = newRate; }
		float& LookSensitivity() { return m_LookSensitivity; }
		float& PanSensitivity() { return m_PanSensitivity; }

	private:
		Camera3D m_Handler;

		float m_PanSensitivity = 1.0f;
		float m_LookSensitivity = 1.0f;
		float m_Distance = 10.0f;
		float m_Smoothing = 0.8f;

		Quaternion m_Rotation;    
		Vector3 m_AngularVelocity = { 0, 0, 0 };              
		
		void SetupCamera(Vector3 Position, Vector3 Target, float Fov);
	};
}