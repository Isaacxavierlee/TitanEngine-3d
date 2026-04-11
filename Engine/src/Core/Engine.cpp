#include "TitanEngine/Core/Engine.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Titan {

    bool Engine::Initialize(const std::string& appName, int width, int height) {
        std::cout << "Initializing TitanEngine..." << std::endl;

        m_Width = width;
        m_Height = height;

        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        // Create window (no OpenGL context - for Vulkan later)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create window!" << std::endl;
            glfwTerminate();
            return false;
        }

        m_IsRunning = true;
        std::cout << "Window created: " << width << "x" << height << std::endl;
        std::cout << "TitanEngine initialized successfully!" << std::endl;

        return true;
    }

    void Engine::Shutdown() {
        std::cout << "Shutting down TitanEngine..." << std::endl;

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

        while (m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {
            // Poll events
            glfwPollEvents();

            // Check for ESC key
            if (glfwGetKey(static_cast<GLFWwindow*>(m_Window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                Quit();
            }

            // Swap buffers (required for window to respond)
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
        }
    }

    void Engine::Quit() {
        std::cout << "Quit requested..." << std::endl;
        m_IsRunning = false;
    }

} // namespace Titan