#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include <memory>

namespace Titan {

    class Renderer {
    public:
        static void BeginScene(const Camera& camera);
        static void EndScene();

        static void Submit(
            const std::shared_ptr<Shader>&      shader,
            const std::shared_ptr<VertexArray>& vertexArray,
            const glm::mat4& modelTransform = glm::mat4(1.f)
        );

        static void SetViewport  (uint32_t x, uint32_t y, uint32_t w, uint32_t h);
        static void SetClearColor(float r, float g, float b, float a = 1.f);
        static void Clear        ();

    private:
        struct SceneData {
            glm::mat4 ViewProjection = glm::mat4(1.f);
        };
        static SceneData s_SceneData;
    };

} // namespace Titan
