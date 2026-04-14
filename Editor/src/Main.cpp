#include <TitanEngine/TitanEngine.h>
#include <imgui.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
// TestLayer — application code only, zero engine internals.
// Replace with HousingLayer, GameLayer, etc.
// ─────────────────────────────────────────────────────────────────────────────
class TestLayer : public Titan::Layer {
public:
    TestLayer()
        : Titan::Layer("TestLayer"),
          m_Camera(60.f, 1280.f / 720.f, 0.1f, 1000.f)
    {}

    void OnAttach() override {
        // Coloured cube: 24 vertices (4 per face), position + colour
        float verts[] = {
            // Front  (blue-white)
            -0.5f,-0.5f, 0.5f, 0.60f,0.80f,1.00f,
             0.5f,-0.5f, 0.5f, 0.60f,0.80f,1.00f,
             0.5f, 0.5f, 0.5f, 0.90f,0.95f,1.00f,
            -0.5f, 0.5f, 0.5f, 0.90f,0.95f,1.00f,
            // Back   (dark blue)
            -0.5f,-0.5f,-0.5f, 0.20f,0.30f,0.60f,
             0.5f,-0.5f,-0.5f, 0.20f,0.30f,0.60f,
             0.5f, 0.5f,-0.5f, 0.30f,0.40f,0.70f,
            -0.5f, 0.5f,-0.5f, 0.30f,0.40f,0.70f,
            // Left   (teal)
            -0.5f,-0.5f,-0.5f, 0.20f,0.70f,0.60f,
            -0.5f,-0.5f, 0.5f, 0.20f,0.70f,0.60f,
            -0.5f, 0.5f, 0.5f, 0.40f,0.90f,0.80f,
            -0.5f, 0.5f,-0.5f, 0.40f,0.90f,0.80f,
            // Right  (coral)
             0.5f,-0.5f, 0.5f, 0.90f,0.40f,0.30f,
             0.5f,-0.5f,-0.5f, 0.90f,0.40f,0.30f,
             0.5f, 0.5f,-0.5f, 1.00f,0.60f,0.50f,
             0.5f, 0.5f, 0.5f, 1.00f,0.60f,0.50f,
            // Top    (light grey)
            -0.5f, 0.5f, 0.5f, 0.95f,0.95f,0.95f,
             0.5f, 0.5f, 0.5f, 0.95f,0.95f,0.95f,
             0.5f, 0.5f,-0.5f, 0.85f,0.85f,0.85f,
            -0.5f, 0.5f,-0.5f, 0.85f,0.85f,0.85f,
            // Bottom (dark)
            -0.5f,-0.5f,-0.5f, 0.15f,0.15f,0.15f,
             0.5f,-0.5f,-0.5f, 0.15f,0.15f,0.15f,
             0.5f,-0.5f, 0.5f, 0.20f,0.20f,0.20f,
            -0.5f,-0.5f, 0.5f, 0.20f,0.20f,0.20f,
        };

        uint32_t idx[] = {
             0, 1, 2,  2, 3, 0,
             4, 6, 5,  6, 4, 7,
             8, 9,10, 10,11, 8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20,
        };

        m_VAO = std::make_shared<Titan::VertexArray>();

        auto vbo = std::make_shared<Titan::VertexBuffer>(verts, sizeof(verts));
        vbo->SetLayout({
            { Titan::ShaderDataType::Float3, "a_Position" },
            { Titan::ShaderDataType::Float3, "a_Color"    },
        });
        m_VAO->AddVertexBuffer(vbo);
        m_VAO->SetIndexBuffer(std::make_shared<Titan::IndexBuffer>(idx, 36));

        const char* vert = R"GLSL(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Color;
            out vec3 v_Color;
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Model;
            void main() {
                v_Color     = a_Color;
                gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
            }
        )GLSL";

        const char* frag = R"GLSL(
            #version 330 core
            in  vec3 v_Color;
            out vec4 FragColor;
            void main() { FragColor = vec4(v_Color, 1.0); }
        )GLSL";

        m_Shader = std::make_shared<Titan::Shader>(vert, frag);
    }

    void OnUpdate(float dt) override {
        // Camera movement
        const float speed    = 4.f * dt;
        const float turnRate = 60.f * dt;
        glm::vec3 pos = m_Camera.GetPosition();

        if (Titan::Input::IsKeyPressed(Titan::Key::W)) pos += m_Camera.GetForward() * speed;
        if (Titan::Input::IsKeyPressed(Titan::Key::S)) pos -= m_Camera.GetForward() * speed;
        if (Titan::Input::IsKeyPressed(Titan::Key::A)) pos -= m_Camera.GetRight()   * speed;
        if (Titan::Input::IsKeyPressed(Titan::Key::D)) pos += m_Camera.GetRight()   * speed;
        if (Titan::Input::IsKeyPressed(Titan::Key::E)) pos.y += speed;
        if (Titan::Input::IsKeyPressed(Titan::Key::Q)) pos.y -= speed;
        m_Camera.SetPosition(pos);

        float yaw   = m_Camera.GetYaw();
        float pitch = m_Camera.GetPitch();
        if (Titan::Input::IsKeyPressed(Titan::Key::Left))  yaw   -= turnRate;
        if (Titan::Input::IsKeyPressed(Titan::Key::Right)) yaw   += turnRate;
        if (Titan::Input::IsKeyPressed(Titan::Key::Up))    pitch  = glm::clamp(pitch + turnRate, -89.f, 89.f);
        if (Titan::Input::IsKeyPressed(Titan::Key::Down))  pitch  = glm::clamp(pitch - turnRate, -89.f, 89.f);
        m_Camera.SetYaw(yaw);
        m_Camera.SetPitch(pitch);

        // Draw
        Titan::Renderer::BeginScene(m_Camera);
        Titan::Renderer::Submit(m_Shader, m_VAO, glm::mat4(1.f));
        Titan::Renderer::EndScene();
    }

    void OnImGuiRender() override {
        ImGui::Begin("TitanEngine");
        glm::vec3 p = m_Camera.GetPosition();
        ImGui::Text("Camera  %.2f  %.2f  %.2f", p.x, p.y, p.z);
        ImGui::Separator();
        ImGui::Text("W A S D      Move");
        ImGui::Text("Q / E        Down / Up");
        ImGui::Text("Arrow keys   Look");
        ImGui::Text("ESC          Quit");
        ImGui::End();
    }

    void OnEvent(Titan::Event& e) override {
        Titan::EventDispatcher d(e);
        d.Dispatch<Titan::WindowResizeEvent>([this](Titan::WindowResizeEvent& ev) {
            if (ev.GetHeight() > 0)
                m_Camera.SetAspectRatio(
                    static_cast<float>(ev.GetWidth()) /
                    static_cast<float>(ev.GetHeight()));
            return false;
        });
    }

private:
    std::shared_ptr<Titan::VertexArray> m_VAO;
    std::shared_ptr<Titan::Shader>      m_Shader;
    Titan::Camera                        m_Camera;
};

// ─────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== TitanEngine Editor ===\n";

    Titan::Engine engine;
    if (!engine.Initialize("TitanEngine Editor", 1280, 720))
        return -1;

    engine.PushLayer(new TestLayer());
    engine.Run();
    engine.Shutdown();
    return 0;
}
