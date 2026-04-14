#include "TitanEngine/Renderer/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Titan {

    Camera::Camera(float fovDeg, float aspect, float nearClip, float farClip)
        : m_Fov(fovDeg), m_Aspect(aspect), m_Near(nearClip), m_Far(farClip) {
        RecalcProjection();
        RecalcView();
    }

    void Camera::SetPosition   (const glm::vec3& p) { m_Position = p;      RecalcView(); }
    void Camera::SetYaw        (float y)             { m_Yaw     = y;      RecalcView(); }
    void Camera::SetPitch      (float p)             { m_Pitch   = p;      RecalcView(); }
    void Camera::SetAspectRatio(float a)             { m_Aspect  = a;  RecalcProjection(); }
    void Camera::SetFov        (float f)             { m_Fov     = f;  RecalcProjection(); }

    glm::vec3 Camera::GetForward() const {
        float yr = glm::radians(m_Yaw), pr = glm::radians(m_Pitch);
        return glm::normalize(glm::vec3{
            std::cos(pr) * std::cos(yr),
            std::sin(pr),
            std::cos(pr) * std::sin(yr)
        });
    }
    glm::vec3 Camera::GetRight() const { return glm::normalize(glm::cross(GetForward(), glm::vec3(0,1,0))); }
    glm::vec3 Camera::GetUp()    const { return glm::normalize(glm::cross(GetRight(), GetForward())); }

    void Camera::RecalcView()       { m_View       = glm::lookAt(m_Position, m_Position + GetForward(), glm::vec3(0,1,0)); }
    void Camera::RecalcProjection() { m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far); }

} // namespace Titan
