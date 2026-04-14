#include "TitanEngine/Core/Input.h"
#include <GLFW/glfw3.h>

namespace Titan {

    void* Input::s_Window = nullptr;

    void Input::SetWindowHandle(void* window) { s_Window = window; }

    bool Input::IsKeyPressed(Key key) {
        if (!s_Window) return false;
        int s = glfwGetKey(static_cast<GLFWwindow*>(s_Window), static_cast<int>(key));
        return s == GLFW_PRESS || s == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseButton button) {
        if (!s_Window) return false;
        int s = glfwGetMouseButton(static_cast<GLFWwindow*>(s_Window), static_cast<int>(button));
        return s == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        if (!s_Window) return { 0.f, 0.f };
        double x, y;
        glfwGetCursorPos(static_cast<GLFWwindow*>(s_Window), &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    float Input::GetMouseX() { return GetMousePosition().x; }
    float Input::GetMouseY() { return GetMousePosition().y; }

} // namespace Titan
