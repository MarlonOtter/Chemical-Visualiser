#include "ArcBall.h"

#include "raymath.h"
#include "raylib.h"
#include <cmath>
#include <algorithm>

namespace Core::Camera
{
	ArcBall::ArcBall(float Fov) : m_Rotation(QuaternionIdentity())
	{
		SetupCamera({ 0,0,0 }, { 0,0,0 }, Fov);
	}

	ArcBall::ArcBall(Vector3 Position, Vector3 Target, float Fov) : m_Rotation(QuaternionIdentity())
	{
		SetupCamera(Position, Target, Fov);
	}

	ArcBall::~ArcBall()
	{

	}

	void ArcBall::Update(float ts, int width, int height)
	{
        const float DefaultLookSensitivity = 5.0f;
        Vector2 delta = GetMouseDelta();

        const float exponent = 60.0f;
        const float decay = std::pow(m_Smoothing, std::max(ts * exponent, 0.0f));


        // Mouse controls
        Quaternion deltaRot = QuaternionIdentity();
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Quaternion prevRot = m_Rotation;

            // yaw
            Quaternion yawRot = QuaternionFromAxisAngle(Vector3{ 0, 1, 0 }, -delta.x * DefaultLookSensitivity * m_LookSensitivity / static_cast<float>(width));

            // pitch
            Matrix quatMat = QuaternionToMatrix(m_Rotation);
            Vector3 right = Vector3Transform(Vector3{ 1, 0, 0 }, quatMat);
            Quaternion pitchRot = QuaternionFromAxisAngle(right, -delta.y * DefaultLookSensitivity * m_LookSensitivity / static_cast<float>(height));

            deltaRot = QuaternionMultiply(pitchRot, yawRot);

            // immediate rotation
            m_Rotation = QuaternionMultiply(pitchRot, m_Rotation);
            m_Rotation = QuaternionMultiply(m_Rotation, yawRot);
            m_Rotation = QuaternionNormalize(m_Rotation);

            Quaternion deltaApplied = QuaternionMultiply(m_Rotation, QuaternionInvert(prevRot));

            // calculate axis angle for angular velocity
            float w = deltaRot.w;
            w = std::clamp(w, -1.0f, 1.0f);
            float angle = 2.0f * std::acos(w);
            float s = std::sqrt(1.0f - w * w);
            Vector3 axis;
            if (s < 1e-6f) {
                axis = Vector3{ 0, 1, 0 };
            }
            else {
                axis = { deltaApplied.x / s, deltaApplied.y / s, deltaApplied.z / s };
            }
            // frame rate
            float invTs = (ts > 1e-6f) ? (1.0f / ts) : 0.0f;
            m_AngularVelocity = { 
                axis.x * angle * invTs,
                axis.y * angle * invTs, 
                axis.z * angle * invTs 
            };
        }
        else
        {
            // apply smoothing
            float angSpeed = Vector3Length(m_AngularVelocity);
            if (angSpeed > 1e-6f)
            {
                m_AngularVelocity *= decay;

                Vector3 axis = Vector3Normalize(m_AngularVelocity);
                float angle = Vector3Length(m_AngularVelocity) * ts;
                Quaternion inertial = QuaternionFromAxisAngle(axis, angle);
                m_Rotation = QuaternionMultiply(inertial, m_Rotation);
                m_Rotation = QuaternionNormalize(m_Rotation);
            }
        }

        // Mouse wheel zoom
        float wheel = GetMouseWheelMove();
        m_Distance -= wheel * 0.5f;
        if (m_Distance < 2.0f) m_Distance = 2.0f;
        if (m_Distance > 50.0f) m_Distance = 50.0f;

        // Calculate forward & up vectors from rotation
        Matrix quatMat = QuaternionToMatrix(m_Rotation);
        Vector3 forward = Vector3Transform(Vector3{ 0, 0, -1 }, quatMat);
        Vector3 up = Vector3Transform(Vector3{ 0, 1, 0 }, quatMat);

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector3 right = Vector3Transform(Vector3{ 1, 0, 0 }, quatMat);
            m_Handler.target += (right * -delta.x * (m_PanSensitivity / static_cast<float>(width)) * m_Distance) + (up * delta.y * (m_PanSensitivity / static_cast<float>(height)) * m_Distance);
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