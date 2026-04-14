#pragma once
#include "KeyCodes.h"
#include <glm/glm.hpp>

namespace Titan {

    class Input {
    public:
        static bool      IsKeyPressed         (Key key);
        static bool      IsMouseButtonPressed  (MouseButton button);
        static glm::vec2 GetMousePosition      ();
        static float     GetMouseX             ();
        static float     GetMouseY             ();

        // Engine-internal: call once after window creation
        static void      SetWindowHandle(void* window);

    private:
        static void* s_Window;
    };

} // namespace Titan
