#include "ArcBall.h"

#include "raymath.h"
#include "raylib.h"

namespace Core::Camera
{
	ArcBall::ArcBall(float Fov)
	{
		SetupCamera({ 0,0,0 }, { 0,0,0 }, Fov);
	}

	ArcBall::ArcBall(Vector3 Position, Vector3 Target, float Fov)
	{
		SetupCamera(Position, Target, Fov);
	}

	ArcBall::~ArcBall()
	{

	}

	void ArcBall::Update(float ts)
	{
        static Quaternion arcballRotation = QuaternionIdentity();

        Vector2 delta = GetMouseDelta();

        // Mouse controls
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            // Yaw (around global up)
            Quaternion yawRot = QuaternionFromAxisAngle(Vector3{ 0, 1, 0 }, -delta.x * m_LookSensitivity);

            // Pitch (around camera's local right)
            Vector3 right = Vector3Transform(Vector3{ 1, 0, 0 }, QuaternionToMatrix(arcballRotation));
            Quaternion pitchRot = QuaternionFromAxisAngle(right, -delta.y * m_LookSensitivity);

            // Combine pitch * yaw * current rotation
            arcballRotation = QuaternionMultiply(pitchRot, arcballRotation);
            arcballRotation = QuaternionMultiply(arcballRotation, yawRot);
            arcballRotation = QuaternionNormalize(arcballRotation); // keep it stable
        }

        // Mouse wheel zoom
        float wheel = GetMouseWheelMove();
        m_Distance -= wheel * 0.5f;
        if (m_Distance < 2.0f) m_Distance = 2.0f;
        if (m_Distance > 50.0f) m_Distance = 50.0f;

        // Calculate forward & up vectors from rotation
        Matrix quatMat = QuaternionToMatrix(arcballRotation);
        Vector3 forward = Vector3Transform(Vector3{ 0, 0, -1 }, quatMat);
        Vector3 up = Vector3Transform(Vector3{ 0, 1, 0 }, quatMat);

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector3 right = Vector3Transform(Vector3{ 1, 0, 0 }, quatMat);
            m_Handler.target += (right * -delta.x * m_PanSensitivity) + (up * delta.y * m_PanSensitivity);
        }

        // Update camera
        m_Handler.position = m_Handler.target + (forward * -m_Distance);
        m_Handler.up = up;

		UpdateCamera(&m_Handler, CAMERA_CUSTOM);
	}

	void ArcBall::SetupCamera(Vector3 Position, Vector3 Target, float Fov)
	{
		m_Handler.position = Position;
		m_Handler.target = Target;
		m_Handler.fovy = Fov;
		m_Handler.projection = CAMERA_PERSPECTIVE;
	}
}