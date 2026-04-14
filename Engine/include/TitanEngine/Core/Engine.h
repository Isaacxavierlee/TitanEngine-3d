#pragma once
#include "TitanEngine/Core/LayerStack.h"
#include "TitanEngine/Events/Event.h"
#include "TitanEngine/Events/WindowEvent.h"
#include <string>
#include <memory>

namespace Titan {

    class GraphicsContext;

    class Engine {
    public:
        Engine();
        ~Engine();

        bool Initialize(const std::string& appName, int width, int height);
        void Run();
        void Shutdown();
        void Quit();

        void PushLayer  (Layer* layer);
        void PushOverlay(Layer* overlay);

        int   GetWidth()  const { return m_Width; }
        int   GetHeight() const { return m_Height; }
        bool  IsRunning() const { return m_IsRunning; }
        void* GetWindow() const { return m_Window; }

        static Engine& Get() { return *s_Instance; }

    private:
        void OnEvent(Event& e);
        bool OnWindowClose (WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        void*  m_Window    = nullptr;
        bool   m_IsRunning = false;
        int    m_Width     = 0;
        int    m_Height    = 0;

        std::unique_ptr<GraphicsContext> m_Context;
        LayerStack m_LayerStack;

        static Engine* s_Instance;
    };

} // namespace Titan
