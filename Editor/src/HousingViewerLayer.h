#pragma once
#include <TitanEngine/TitanEngine.h>
#include <imgui.h>
#include <memory>

class HousingViewerLayer : public Titan::Layer {
public:
    HousingViewerLayer()
        : Titan::Layer("HousingViewer"),
        m_Camera(60.f, 1280.f / 720.f, 0.1f, 1000.f)
    {
    }

    void OnAttach()         override;
    void OnDetach()         override;
    void OnUpdate(float dt) override;
    void OnImGuiRender()    override;
    void OnEvent(Titan::Event& e) override;

private:
    Titan::Camera                       m_Camera;
    std::shared_ptr<Titan::VertexArray> m_VAO;
    std::shared_ptr<Titan::Shader>      m_Shader;
};