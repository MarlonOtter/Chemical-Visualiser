#include "Camera2D.h"

#include <Slate/Core.h>
#include <algorithm>

Camera2D::Camera2D()
	: Camera(Orthographic)
{

}

void Camera2D::Update(float ts)
{
	// TODO Add Camera Smoothing Back

	Slate::InputsHandler& Inputs = Slate::Application::Get().Inputs();

	if (Inputs.IsMouseButtonDown(Slate::MouseButton::LEFT))
	{
		Position += glm::vec3(Inputs.MouseDeltaPosition() * (1.0f / Zoom), 0.0f);
	}
	Target = Position + glm::vec3(0.0f, 0.0f, -1.0f);
	
	Zoom *= std::clamp(Inputs.MouseScroll().y, -1.0f, 1.0f) * 0.1f + 1.0f;
	
	OrthographicScale = Zoom;
}