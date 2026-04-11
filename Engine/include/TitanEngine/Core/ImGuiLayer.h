#pragma once

struct GLFWwindow;

namespace Titan {

    class ImGuiLayer {
    public:
        static ImGuiLayer& Get();

        void Init(GLFWwindow* window);
        void Shutdown();
        void NewFrame();
        void Render();

    private:
        ImGuiLayer() = default;
        ~ImGuiLayer() = default;
        ImGuiLayer(const ImGuiLayer&) = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;

        bool m_Initialized = false;
    };

} // namespace Titan