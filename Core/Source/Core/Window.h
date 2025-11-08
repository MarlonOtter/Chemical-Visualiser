#pragma once

#include <string>

#include "raylib.h"

namespace Core
{
	struct WindowSpecification
	{
		std::string Title;
		uint32_t Width = 1280;
		uint32_t Height = 720;
		Color ClearColor = BLACK;
		bool IsResizeable = true;
		bool VSync = true;
		std::string IconPath = "";
	};

	class Window {
	public:
		Window(const WindowSpecification& specification = WindowSpecification());
		~Window();
		
		void Create();
		void Destroy();

		bool ShouldClose();

		Vector2 GetSize();
		Vector2 GetPosition();

		void SetTitle(const std::string Title);
		void Resize(const uint32_t width, const uint32_t height);
		void SetVSync(bool enabled);
		void SetResizeable(bool enabled);

	private:
		WindowSpecification m_Specification;
		bool m_Initialized = false;
	};
}