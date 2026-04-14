#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Titan {

    class Shader {
    public:
        Shader(const std::string& vertSrc, const std::string& fragSrc);
        static Shader FromFiles(const std::string& vertPath, const std::string& fragPath);
        ~Shader();

        void Bind()   const;
        void Unbind() const;

        void SetBool  (const std::string& name, bool value);
        void SetInt   (const std::string& name, int value);
        void SetFloat (const std::string& name, float value);
        void SetFloat2(const std::string& name, const glm::vec2& v);
        void SetFloat3(const std::string& name, const glm::vec3& v);
        void SetFloat4(const std::string& name, const glm::vec4& v);
        void SetMat3  (const std::string& name, const glm::mat3& m);
        void SetMat4  (const std::string& name, const glm::mat4& m);

    private:
        uint32_t Compile(uint32_t type, const std::string& src);
        int      Loc    (const std::string& name) const;

        uint32_t m_RendererID = 0;
    };

} // namespace Titan
