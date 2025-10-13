#include "Window.h"

namespace Core
{
	Window::Window(const WindowSpecification& specification)
		: m_Specification(specification)
	{
	}

	Window::~Window()
	{
		Destroy();
	}

	void Window::Create()
	{
		InitWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str());
		SetResizeable(m_Specification.IsResizeable);
		SetVSync(m_Specification.VSync);
		m_Initialized = true;
	}

	void Window::Destroy()
	{
		if (!m_Initialized || !IsWindowReady())
			return;

		CloseWindow();
	}

	bool Window::ShouldClose()
	{
		return WindowShouldClose();
	}

	Vector2 Window::GetSize()
	{
		return Vector2(GetScreenWidth(), GetScreenHeight());
	}

	Vector2 Window::GetPosition()
	{
		return GetWindowPosition();
	}

	void Window::SetTitle(const std::string Title)
	{
		m_Specification.Title = Title;
		SetWindowTitle(m_Specification.Title.c_str());
	}

	void Window::Resize(const uint32_t width, const uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
	}

	void Window::SetVSync(bool enabled)
	{
		m_Specification.VSync = enabled;
		SetTargetFPS(m_Specification.VSync ? GetMonitorRefreshRate(GetCurrentMonitor()) : 0);
	}

	void Window::SetResizeable(bool enabled)
	{
		m_Specification.IsResizeable = enabled;
		SetWindowState(m_Specification.IsResizeable ? FLAG_WINDOW_RESIZABLE : 0);
	}
}