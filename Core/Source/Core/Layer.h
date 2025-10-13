#pragma once

#include "Event.h"
#include <memory>

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
		virtual void OnComposite() {}
		
		// Implement later
		virtual void FixedUpdate(float fixedDeltaTime) {}

		virtual void OnEvent(Event& event) {}


		template<std::derived_from<Layer> T, typename ... Args>
		void TransitionTo(Args&& ... args) {
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);
	};
}