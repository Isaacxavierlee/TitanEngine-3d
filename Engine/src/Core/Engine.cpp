// Include windows.h BEFORE any OpenGL headers
#include <windows.h>

// Then OpenGL headers
#include <GL/gl.h>
#include <GLFW/glfw3.h>

// Then engine headers
#include "TitanEngine/Core/Engine.h"
#include "TitanEngine/Core/ImGuiLayer.h"
#include "TitanEngine/Math/Vector3.h"
#include "TitanEngine/Math/Matrix4.h"
#include <imgui.h>
#include <iostream>
#include <chrono>

namespace Titan {

    Engine::Engine() = default;
    Engine::~Engine() = default;

    void TestMath() {
        Vector3 pos(1, 2, 3);
        Vector3 vel(0.1f, 0.2f, 0.3f);
        Vector3 newPos = pos + vel;
        std::cout << "[TEST] Vector3: " << newPos.x << "," << newPos.y << "," << newPos.z << std::endl;

        Matrix4 trans = Matrix4::Translation(Vector3(1, 2, 3));
        Vector3 point = trans.TransformPoint(Vector3::Zero());
        std::cout << "[TEST] Matrix4: " << point.x << "," << point.y << "," << point.z << std::endl;
    }

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

        // Test math library
        TestMath();

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