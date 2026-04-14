#include "TitanEngine/Renderer/GraphicsContext.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace Titan {

    GraphicsContext::GraphicsContext(GLFWwindow* window) : m_Window(window) {}

    void GraphicsContext::Init() {
        glfwMakeContextCurrent(m_Window);
        GLenum err = glewInit();
        if (err != GLEW_OK)
            throw std::runtime_error(std::string("[GraphicsContext] GLEW failed: ")
                + reinterpret_cast<const char*>(glewGetErrorString(err)));

        std::cout << "[GraphicsContext] Vendor   : " << glGetString(GL_VENDOR)   << "\n"
                  << "[GraphicsContext] Renderer : " << glGetString(GL_RENDERER) << "\n"
                  << "[GraphicsContext] OpenGL   : " << glGetString(GL_VERSION)  << "\n";

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void GraphicsContext::SwapBuffers() { glfwSwapBuffers(m_Window); }

} // namespace Titan
