#include "TitanEngine/Renderer/Shader.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Titan {

    Shader::Shader(const std::string& vertSrc, const std::string& fragSrc) {
        uint32_t vert = Compile(GL_VERTEX_SHADER,   vertSrc);
        uint32_t frag = Compile(GL_FRAGMENT_SHADER, fragSrc);
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vert);
        glAttachShader(m_RendererID, frag);
        glLinkProgram(m_RendererID);
        int ok = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetProgramInfoLog(m_RendererID, 1024, nullptr, log);
            glDeleteProgram(m_RendererID);
            throw std::runtime_error(std::string("[Shader] Link error:\n") + log);
        }
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    Shader Shader::FromFiles(const std::string& vp, const std::string& fp) {
        auto read = [](const std::string& path) {
            std::ifstream f(path);
            if (!f) throw std::runtime_error("[Shader] Cannot open: " + path);
            std::ostringstream ss; ss << f.rdbuf(); return ss.str();
        };
        return Shader(read(vp), read(fp));
    }

    Shader::~Shader() { glDeleteProgram(m_RendererID); }
    void Shader::Bind()   const { glUseProgram(m_RendererID); }
    void Shader::Unbind() const { glUseProgram(0); }

    uint32_t Shader::Compile(uint32_t type, const std::string& src) {
        uint32_t id = glCreateShader(type);
        const char* c = src.c_str();
        glShaderSource(id, 1, &c, nullptr);
        glCompileShader(id);
        int ok = 0; glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetShaderInfoLog(id, 1024, nullptr, log);
            glDeleteShader(id);
            throw std::runtime_error(std::string("[Shader] Compile error:\n") + log);
        }
        return id;
    }

    int Shader::Loc(const std::string& name) const {
        return glGetUniformLocation(m_RendererID, name.c_str());
    }

    void Shader::SetBool  (const std::string& n, bool v)             { glUniform1i (Loc(n), v ? 1 : 0); }
    void Shader::SetInt   (const std::string& n, int v)              { glUniform1i (Loc(n), v); }
    void Shader::SetFloat (const std::string& n, float v)            { glUniform1f (Loc(n), v); }
    void Shader::SetFloat2(const std::string& n, const glm::vec2& v) { glUniform2fv(Loc(n), 1, glm::value_ptr(v)); }
    void Shader::SetFloat3(const std::string& n, const glm::vec3& v) { glUniform3fv(Loc(n), 1, glm::value_ptr(v)); }
    void Shader::SetFloat4(const std::string& n, const glm::vec4& v) { glUniform4fv(Loc(n), 1, glm::value_ptr(v)); }
    void Shader::SetMat3  (const std::string& n, const glm::mat3& m) { glUniformMatrix3fv(Loc(n), 1, GL_FALSE, glm::value_ptr(m)); }
    void Shader::SetMat4  (const std::string& n, const glm::mat4& m) { glUniformMatrix4fv(Loc(n), 1, GL_FALSE, glm::value_ptr(m)); }

} // namespace Titan
