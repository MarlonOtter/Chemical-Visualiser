#pragma once

#include "Event.h"

namespace Core
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;
		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void EarlyUpdate(float deltaTime) {}
		virtual void Update(float deltaTime) {}
		virtual void LateUpdate(float deltaTime) {}

		virtual void OnRender() {}
		
		// Implement later
		virtual void FixedUpdate(float fixedDeltaTime) {}

		virtual void OnEvent(Event& event) {}
	};
}