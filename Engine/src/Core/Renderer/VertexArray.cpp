#include "TitanEngine/Renderer/VertexArray.h"
#include <GL/glew.h>

namespace Titan {

    static GLenum ToGLBaseType(ShaderDataType t) {
        switch (t) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:  return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:  return GL_INT;
            case ShaderDataType::Bool:  return GL_BOOL;
            default:                    return GL_FLOAT;
        }
    }

    VertexArray::VertexArray()  { glGenVertexArrays(1, &m_RendererID); }
    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }
    void VertexArray::Bind()    const { glBindVertexArray(m_RendererID); }
    void VertexArray::Unbind()  const { glBindVertexArray(0); }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) {
        glBindVertexArray(m_RendererID);
        vb->Bind();
        for (const auto& el : vb->GetLayout()) {
            glEnableVertexAttribArray(m_AttribIndex);
            glVertexAttribPointer(
                m_AttribIndex,
                static_cast<GLint>(el.GetComponentCount()),
                ToGLBaseType(el.Type),
                el.Normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(vb->GetLayout().GetStride()),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(el.Offset))
            );
            m_AttribIndex++;
        }
        m_VBOs.push_back(vb);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) {
        glBindVertexArray(m_RendererID);
        ib->Bind();
        m_IBO = ib;
    }

} // namespace Titan
