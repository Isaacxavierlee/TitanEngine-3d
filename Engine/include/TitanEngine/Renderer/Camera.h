#pragma once
#include <glm/glm.hpp>

namespace Titan {

    class Camera {
    public:
        Camera(float fovDeg, float aspect, float nearClip, float farClip);

        void SetPosition   (const glm::vec3& pos);
        void SetYaw        (float yaw);
        void SetPitch      (float pitch);
        void SetAspectRatio(float aspect);
        void SetFov        (float fovDeg);

        const glm::vec3& GetPosition()       const { return m_Position; }
        float            GetYaw()            const { return m_Yaw; }
        float            GetPitch()          const { return m_Pitch; }
        glm::vec3        GetForward()        const;
        glm::vec3        GetRight()          const;
        glm::vec3        GetUp()             const;

        const glm::mat4& GetView()           const { return m_View; }
        const glm::mat4& GetProjection()     const { return m_Projection; }
        glm::mat4        GetViewProjection() const { return m_Projection * m_View; }

    private:
        void RecalcView();
        void RecalcProjection();

        glm::vec3 m_Position   = { 0.f, 1.f, 5.f };
        float     m_Yaw        = -90.f;
        float     m_Pitch      =   0.f;
        float     m_Fov        =  60.f;
        float     m_Aspect     =  16.f / 9.f;
        float     m_Near       =   0.1f;
        float     m_Far        = 1000.f;

        glm::mat4 m_View       = glm::mat4(1.f);
        glm::mat4 m_Projection = glm::mat4(1.f);
    };

} // namespace Titan
