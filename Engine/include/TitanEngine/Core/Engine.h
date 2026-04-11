#pragma once
#include <string>

namespace Titan {

    class Engine {
    public:
        bool Initialize(const std::string& appName, int width, int height);
        void Shutdown();
        void Run();
        void Quit();
        bool IsRunning() const { return m_IsRunning; }

    private:
        void* m_Window = nullptr;  // GLFWwindow* (void* to avoid including GLFW in header)
        bool m_IsRunning = false;
        int m_Width = 0;
        int m_Height = 0;
    };

} // namespace Titan