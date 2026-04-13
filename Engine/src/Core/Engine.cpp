#include "TitanEngine/Core/Engine.h"
#include "TitanEngine/Core/ImGuiLayer.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

// Simple OpenGL includes for Windows
#include <windows.h>
#include <GL/gl.h>

namespace Titan {

    Engine::Engine() = default;
    Engine::~Engine() = default;

    bool Engine::Initialize(const std::string& appName, int width, int height) {
        std::cout << "Initializing TitanEngine..." << std::endl;

        m_Width = width;
        m_Height = height;

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create window!" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window));
        glfwSwapInterval(1);

        ImGuiLayer::Get().Initialize(static_cast<GLFWwindow*>(m_Window));

        m_IsRunning = true;
        std::cout << "TitanEngine initialized!" << std::endl;
        return true;
    }

    void Engine::Run() {
        std::cout << "Engine running. Press ESC to exit..." << std::endl;

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {

            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            (void)deltaTime;

            glfwPollEvents();

            if (glfwGetKey(static_cast<GLFWwindow*>(m_Window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                Quit();
            }

            // Clear screen
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGuiLayer::Get().NewFrame();
            ImGui::ShowDemoWindow();
            ImGuiLayer::Get().Render();

            glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
        }
    }

    void Engine::Quit() {
        std::cout << "Quit requested..." << std::endl;
        m_IsRunning = false;
    }

    void Engine::Shutdown() {
        std::cout << "Shutting down TitanEngine..." << std::endl;

        ImGuiLayer::Get().Shutdown();

        if (m_Window) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
        }

        glfwTerminate();
        std::cout << "TitanEngine shutdown complete!" << std::endl;
    }

} // namespace Titan