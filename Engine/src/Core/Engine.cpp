#include "TitanEngine/Core/Engine.h"
#include "TitanEngine/Core/ImGuiLayer.h"
#include "TitanEngine/Core/Input.h"
#include "TitanEngine/Renderer/GraphicsContext.h"
#include "TitanEngine/Events/KeyEvent.h"
#include "TitanEngine/Events/MouseEvent.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include <chrono>

namespace Titan {

    Engine* Engine::s_Instance = nullptr;

    Engine::Engine() { s_Instance = this; }
    Engine::~Engine() = default;

    bool Engine::Initialize(const std::string& appName, int width, int height) {
        std::cout << "[Engine] Initializing " << appName
            << " (" << width << "x" << height << ")\n";
        m_Width = width;
        m_Height = height;

        if (!glfwInit()) {
            std::cerr << "[Engine] GLFW init failed\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "[Engine] Failed to create window\n";
            glfwTerminate();
            return false;
        }

        m_Context = std::make_unique<GraphicsContext>(static_cast<GLFWwindow*>(m_Window));
        m_Context->Init();

        glfwSwapInterval(1);
        Input::SetWindowHandle(m_Window);
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), this);

        glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w) {
                WindowCloseEvent e;
                static_cast<Engine*>(glfwGetWindowUserPointer(w))->OnEvent(e);
            });

        glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w, int width, int height) {
                WindowResizeEvent e(width, height);
                static_cast<Engine*>(glfwGetWindowUserPointer(w))->OnEvent(e);
            });

        glfwSetKeyCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w, int key, int, int action, int) {
                Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(w));
                switch (action) {
                case GLFW_PRESS: { KeyPressedEvent  e(static_cast<Key>(key), false); eng->OnEvent(e); break; }
                case GLFW_REPEAT: { KeyPressedEvent  e(static_cast<Key>(key), true);  eng->OnEvent(e); break; }
                case GLFW_RELEASE: { KeyReleasedEvent e(static_cast<Key>(key));         eng->OnEvent(e); break; }
                }
            });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w, int button, int action, int) {
                Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(w));
                if (action == GLFW_PRESS) {
                    MouseButtonPressedEvent  e(static_cast<MouseButton>(button)); eng->OnEvent(e);
                }
                else {
                    MouseButtonReleasedEvent e(static_cast<MouseButton>(button)); eng->OnEvent(e);
                }
            });

        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w, double x, double y) {
                MouseMovedEvent e(static_cast<float>(x), static_cast<float>(y));
                static_cast<Engine*>(glfwGetWindowUserPointer(w))->OnEvent(e);
            });

        glfwSetScrollCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* w, double xOff, double yOff) {
                MouseScrolledEvent e(static_cast<float>(xOff), static_cast<float>(yOff));
                static_cast<Engine*>(glfwGetWindowUserPointer(w))->OnEvent(e);
            });

        ImGuiLayer::Get().Initialize(static_cast<GLFWwindow*>(m_Window));

        m_IsRunning = true;
        std::cout << "[Engine] Ready\n";
        return true;
    }

    void Engine::Run() {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {
            auto now = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float>(now - lastTime).count();
            lastTime = now;

            glfwPollEvents();

            glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(dt);

            ImGuiLayer::Get().NewFrame();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            ImGuiLayer::Get().Render();

            m_Context->SwapBuffers();
        }
    }

    void Engine::Quit() { m_IsRunning = false; }

    void Engine::Shutdown() {
        std::cout << "[Engine] Shutting down\n";
        ImGuiLayer::Get().Shutdown();
        if (m_Window) glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
        glfwTerminate();
        std::cout << "[Engine] Done\n";
    }

    void Engine::PushLayer(Layer* l) { m_LayerStack.PushLayer(l); }
    void Engine::PushOverlay(Layer* o) { m_LayerStack.PushOverlay(o); }

    void Engine::OnEvent(Event& e) {
        EventDispatcher d(e);
        d.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& ev) { return OnWindowClose(ev);  });
        d.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& ev) { return OnWindowResize(ev); });

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        }
    }

    bool Engine::OnWindowClose(WindowCloseEvent&) { Quit(); return true; }

    bool Engine::OnWindowResize(WindowResizeEvent& e) {
        m_Width = e.GetWidth();
        m_Height = e.GetHeight();
        glViewport(0, 0, m_Width, m_Height);
        return false;
    }

} // namespace Titan