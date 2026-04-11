#include "TitanEngine/Core/Engine.h"
#include "TitanEngine/Core/ImGuiLayer.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Titan {

    bool Engine::Initialize(const std::string& appName, int width, int height) {
        std::cout << "Initializing TitanEngine..." << std::endl;

        m_Width = width;
        m_Height = height;

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        // Set OpenGL version (3.3 is stable)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Double buffering and vsync
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        m_Window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create window!" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window));

        // Enable vsync (prevents flickering)
        glfwSwapInterval(1);

        // Initialize ImGui
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
        std::cout << "Engine running. Press ESC to exit..." << std::endl;
        std::cout << "ImGui demo window should appear!" << std::endl;

        while (m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {
            // Poll events
            glfwPollEvents();

            // Check for ESC key
            if (glfwGetKey(static_cast<GLFWwindow*>(m_Window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                Quit();
            }

            // Get window size for viewport
            int display_w, display_h;
            glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_Window), &display_w, &display_h);

            // Set viewport
            glViewport(0, 0, display_w, display_h);

            // Clear screen with a solid color (dark gray)
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Start ImGui frame
            ImGuiLayer::Get().NewFrame();

            // Draw ImGui UI (this will overlay on top of the cleared screen)
            ImGui::ShowDemoWindow();

            // Render ImGui
            ImGuiLayer::Get().Render();

            // Swap buffers (this actually displays what we've drawn)
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
        }
    }

    void Engine::Quit() {
        std::cout << "Quit requested..." << std::endl;
        m_IsRunning = false;
    }

} // namespace Titan