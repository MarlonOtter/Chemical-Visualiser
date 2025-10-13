#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

#include "Layer.h"
#include "Window.h"

#include "raymath.h"

namespace Core
{
    void PrintHello();

    struct ApplicationSpecification
    {
		std::string Name = "Core Application";
        WindowSpecification WindowSpec;
    };


    class Application {
    public:
        Application(ApplicationSpecification specification);
        ~Application();

        void Run();
        void Stop();

        template<typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer() {
            m_LayerStack.push_back(std::make_unique<TLayer>());
        }

        static Application& Get();
        static float GetTime();

    private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = true;

        std::vector<std::unique_ptr<Layer>> m_LayerStack;

    };
}