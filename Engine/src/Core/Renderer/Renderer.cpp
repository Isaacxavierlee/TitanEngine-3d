#include "TitanEngine/Renderer/Renderer.h"
#include <GL/glew.h>

namespace Titan {

    Renderer::SceneData Renderer::s_SceneData;

    void Renderer::BeginScene(const Camera& camera) {
        s_SceneData.ViewProjection = camera.GetViewProjection();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(
        const std::shared_ptr<Shader>&      shader,
        const std::shared_ptr<VertexArray>& vao,
        const glm::mat4&                    model)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData.ViewProjection);
        shader->SetMat4("u_Model",          model);
        vao->Bind();
        glDrawElements(GL_TRIANGLES,
            static_cast<GLsizei>(vao->GetIndexBuffer()->GetCount()),
            GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
    }
    void Renderer::SetClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
    void Renderer::Clear()  { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

} // namespace Titan
