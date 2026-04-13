#pragma once
#include <string>

namespace Titan {

    class Engine {
    public:
        Engine();
        ~Engine();

        bool Initialize(const std::string& appName, int width, int height);
        void Shutdown();
        void Run();
        void Quit();
        bool IsRunning() const { return m_IsRunning; }

    private:
        void* m_Window = nullptr;
        bool m_IsRunning = false;
        int m_Width = 0;
        int m_Height = 0;
    };

} // namespace Titan