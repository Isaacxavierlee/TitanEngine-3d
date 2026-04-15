#pragma once
#include "Engine.h"
#include "Layer.h"
#include <string>

namespace Titan {

    struct AppConfig {
        std::string Title = "TitanEngine App";
        int         Width = 1280;
        int         Height = 720;
    };

    class Application {
    public:
        virtual ~Application() = default;

        void Run(const AppConfig& config = {}) {
            if (!m_Engine.Initialize(config.Title, config.Width, config.Height))
                return;
            OnInit();
            m_Engine.Run();
            OnShutdown();
            m_Engine.Shutdown();
        }

        virtual void OnInit() = 0;
        virtual void OnShutdown() {}

    protected:
        void PushLayer(Layer* l) { m_Engine.PushLayer(l); }
        void PushOverlay(Layer* o) { m_Engine.PushOverlay(o); }

    private:
        Engine m_Engine;
    };

} // namespace Titan