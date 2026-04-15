#include "HousingViewerLayer.h"

void HousingViewerLayer::OnAttach() {}
void HousingViewerLayer::OnDetach() {}

void HousingViewerLayer::OnUpdate(float dt) {
    Titan::Renderer::BeginScene(m_Camera);
    Titan::Renderer::EndScene();
}

void HousingViewerLayer::OnImGuiRender() {
    ImGui::Begin("Housing Viewer");
    glm::vec3 p = m_Camera.GetPosition();
    ImGui::Text("Camera  %.2f  %.2f  %.2f", p.x, p.y, p.z);
    ImGui::End();
}

void HousingViewerLayer::OnEvent(Titan::Event& e) {
    Titan::EventDispatcher d(e);
    d.Dispatch<Titan::WindowResizeEvent>([this](Titan::WindowResizeEvent& ev) {
        if (ev.GetHeight() > 0)
            m_Camera.SetAspectRatio(
                static_cast<float>(ev.GetWidth()) /
                static_cast<float>(ev.GetHeight()));
        return false;
        });
}