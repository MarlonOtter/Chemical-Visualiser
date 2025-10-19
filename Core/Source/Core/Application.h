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
        bool eventRendering = false;
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

        template<typename TLayer>
            requires(std::is_base_of_v<Layer, TLayer>)
        TLayer* GetLayer() {
            for (const auto& layer : m_LayerStack) {
                if (auto typed = dynamic_cast<TLayer*>(layer.get())) {
                    return typed;
                }
            }
            return static_cast<TLayer*>(nullptr);
        }

        static Application& Get();
        static float GetTime();
        Vector2 GetWindowSize();

        void RequestRedraw() { m_NeedsRedraw = true; };

        std::vector<std::unique_ptr<Layer>> m_LayerStack;

    private:

        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_NeedsRedraw = true;
    };
}