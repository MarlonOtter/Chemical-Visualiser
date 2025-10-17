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

		Vector3 GetPosition() { return m_Handler.position; }
		Vector3 GetTarget() { return m_Handler.target; }
		void SetTarget(Vector3 Target) { m_Handler.target = Target; }
		Camera3D& GetHandler() { return m_Handler; }

		void Update(float ts);
		
		void SetLookSensitivity(float Sensitivity) { m_LookSensitivity = Sensitivity; }
		float GetLookSensitivity() { return m_LookSensitivity; }

	private:
		Camera3D m_Handler;

		float m_PanSensitivity = 0.005f;
		float m_LookSensitivity = 0.005f;
		float m_Distance = 10.0f;
		
		void SetupCamera(Vector3 Position, Vector3 Target, float Fov);
	};
}