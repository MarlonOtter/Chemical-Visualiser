#include "Application.h"

namespace Core
{
    static Application* s_Application = nullptr;

    void PrintHello()
    {
        std::cout << "Hello from Core!" << std::endl;
    }

    Application::Application(ApplicationSpecification specification)
        : m_Specification(specification)
    {
        s_Application = this;

        if (specification.WindowSpec.Title.empty()) {
			m_Specification.WindowSpec.Title = m_Specification.Name;
        }

        m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
        m_Window->Create();

	}

    Application::~Application()
    {
        m_Window->Destroy();
		s_Application = nullptr;
    }

    void Application::Run()
    {
		m_Running = true;

		float lastTime = GetTime();

        while (m_Running) {
            if (m_Window->ShouldClose()) {
                Stop();
                break;
			}

			float currentTime = GetTime();
            float timestep = Clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            // These have to be done in sequence
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->EarlyUpdate(timestep);

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->Update(timestep);

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->LateUpdate(timestep);

            // Can be on seperate thread
            BeginDrawing();

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();
        
			EndDrawing();
        }
    }

    void Application::Stop()
    {
		m_Running = false;
	}

    Application& Application::Get()
    {
        assert(s_Application);
        return *s_Application;;
    }


    float Application::GetTime()
    {
        return static_cast<float>(::GetTime());
	}

}