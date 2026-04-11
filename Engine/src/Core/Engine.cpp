#include "TitanEngine/Core/Engine.h"
#include "TitanEngine/Core/ImGuiLayer.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Titan {

    bool Engine::Initialize(const std::string& appName, int width, int height) {
        std::cout << "Initializing TitanEngine with Vulkan..." << std::endl;

        m_Width = width;
        m_Height = height;

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        // Vulkan requires no graphics API context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create window!" << std::endl;
            glfwTerminate();
            return false;
        }

        // Initialize ImGui (which initializes Vulkan)
        ImGuiLayer::Get().Init(static_cast<GLFWwindow*>(m_Window));

        m_IsRunning = true;
        std::cout << "Window created: " << width << "x" << height << std::endl;
        std::cout << "TitanEngine initialized successfully!" << std::endl;

        return true;
    }

    void Engine::Shutdown() {
        std::cout << "Shutting down TitanEngine..." << std::endl;

        ImGuiLayer::Get().Shutdown();

        if (m_Window) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
            m_Window = nullptr;
        }

        glfwTerminate();
        m_IsRunning = false;

        std::cout << "TitanEngine shutdown complete!" << std::endl;
    }

    void Engine::Run() {
        std::cout << "Engine running with Vulkan. Press ESC to exit..." << std::endl;

        while (m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {
            glfwPollEvents();

            if (glfwGetKey(static_cast<GLFWwindow*>(m_Window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                Quit();
            }

            // ImGui handles all rendering
            ImGuiLayer::Get().NewFrame();
            ImGui::ShowDemoWindow();
            ImGuiLayer::Get().Render();
        }

        ImGuiLayer::Get().Shutdown();
    }

    void Engine::Quit() {
        std::cout << "Quit requested..." << std::endl;
        m_IsRunning = false;
    }

} // namespace Titan