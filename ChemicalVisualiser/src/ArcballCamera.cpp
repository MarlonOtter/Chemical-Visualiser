#include "ArcballCamera.h"

#include <Slate/Math.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <Slate/Core.h>
#include <algorithm>

ArcballCamera::ArcballCamera()
    : Camera(Perspective), m_Rotation(glm::identity<glm::quat>())
{

}

void ArcballCamera::Update(float ts)
{
    CalculateRotation(ts);

	Camera::Update(ts);
}

void ArcballCamera::CalculateRotation(float ts)
{
    Slate::InputsHandler& Inputs = Slate::Application::Get().Inputs();
    const float DefaultLookSensitivity = 5.0f;
    glm::vec2 delta = Inputs.MouseDeltaPosition();

    const float exponent = 60.0f;
    const float decay = std::pow(m_Smoothing, std::max(ts * exponent, 0.0f));


    // Mouse controls
    glm::quat deltaRot = glm::identity<glm::quat>();
    if (Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT))
    {
        glm::quat prevRot = m_Rotation;

        // yaw
        glm::quat yawRot = glm::angleAxis(-delta.x * DefaultLookSensitivity * m_LookSensitivity / static_cast<float>(Size.x), glm::vec3{ 0, 1, 0 });

        // pitch
        glm::mat4 quatMat = glm::toMat4(m_Rotation);
        glm::vec3 right = glm::vec3(glm::vec4{ 1, 0, 0, 1.0f } * quatMat);
        glm::quat pitchRot = glm::angleAxis(-delta.y * DefaultLookSensitivity * m_LookSensitivity / static_cast<float>(Size.y), right);

        deltaRot = pitchRot * yawRot;

        // immediate rotation
        m_Rotation = pitchRot * m_Rotation;
        m_Rotation = m_Rotation * yawRot;
        m_Rotation = glm::normalize(m_Rotation);

        glm::quat deltaApplied = m_Rotation * glm::inverse(prevRot);

        // calculate axis angle for angular velocity
        float w = deltaRot.w;
        w = std::clamp(w, -1.0f, 1.0f);
        float angle = 2.0f * std::acos(w);
        float s = std::sqrt(1.0f - w * w);
        glm::vec3 axis;
        if (s < 1e-6f) {
            axis = { 0, 1, 0 };
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
        float angSpeed = glm::length(m_AngularVelocity);
        if (angSpeed > 1e-6f)
        {
            m_AngularVelocity *= decay;

            glm::vec3 axis = glm::normalize(m_AngularVelocity);
            float angle = glm::length(m_AngularVelocity) * ts;
            glm::quat inertial = glm::angleAxis(angle, axis);
            m_Rotation = inertial * m_Rotation;
            m_Rotation = glm::normalize(m_Rotation);
        }
    }

    // Mouse wheel zoom
    m_Distance = glm::mix(m_Distance, m_TargetDistance, ts * 30.0f * (1.0f - std::min(decay, 0.90f)));

    float wheel = Inputs.MouseScroll().y;
    m_TargetDistance -= wheel * 0.5f;
    if (m_Distance < 2.0f) m_Distance = 2.0f;
    if (m_Distance > 50.0f) m_Distance = 50.0f;


    // Calculate forward & up vectors from rotation
    glm::mat4 quatMat = glm::toMat4(m_Rotation);
    glm::vec3 forward = glm::vec3(glm::vec4{ 0.0f, 0.0f, -1.0f, 1.0f } *quatMat);
    glm::vec3 up = glm::vec3(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f } *quatMat);

    // Panning
    Target = glm::mix(Target, m_TargetPanPosition, ts * 30.0f * (1.0f - std::min(decay, 0.90f)));
    if (Inputs.IsMouseButtonDown(Slate::MouseButton::MIDDLE) && !Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT))
    {
        glm::vec3 right = glm::vec3(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f } * quatMat);
        m_TargetPanPosition += (right * -delta.x * (m_PanSensitivity / static_cast<float>(Size.x)) * m_Distance) + (up * delta.y * (m_PanSensitivity / static_cast<float>(Size.y)) * m_Distance);
    }

    // Update camera
    Position = Target + (forward * -m_Distance);
    Up = up;

}