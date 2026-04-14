#pragma once

struct GLFWwindow;

namespace Titan {

    class GraphicsContext {
    public:
        explicit GraphicsContext(GLFWwindow* window);
        void Init();
        void SwapBuffers();

    private:
        GLFWwindow* m_Window;
    };

} // namespace Titan
