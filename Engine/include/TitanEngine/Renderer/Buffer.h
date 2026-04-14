#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Titan {

    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType t) {
        switch (t) {
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 8;
            case ShaderDataType::Float3: return 12;
            case ShaderDataType::Float4: return 16;
            case ShaderDataType::Mat3:   return 36;
            case ShaderDataType::Mat4:   return 64;
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 8;
            case ShaderDataType::Int3:   return 12;
            case ShaderDataType::Int4:   return 16;
            case ShaderDataType::Bool:   return 1;
            default:                     return 0;
        }
    }

    struct BufferElement {
        std::string    Name;
        ShaderDataType Type       = ShaderDataType::None;
        uint32_t       Size       = 0;
        uint32_t       Offset     = 0;
        bool           Normalized = false;

        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized) {}

        uint32_t GetComponentCount() const {
            switch (Type) {
                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3:   return 3;
                case ShaderDataType::Mat4:   return 4;
                case ShaderDataType::Int:    return 1;
                case ShaderDataType::Int2:   return 2;
                case ShaderDataType::Int3:   return 3;
                case ShaderDataType::Int4:   return 4;
                case ShaderDataType::Bool:   return 1;
                default:                     return 0;
            }
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements) { CalculateOffsets(); }

        uint32_t                          GetStride()   const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator       begin()       { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator       end()         { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end()   const { return m_Elements.end(); }

    private:
        void CalculateOffsets() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& el : m_Elements) {
                el.Offset  = offset;
                offset    += el.Size;
                m_Stride  += el.Size;
            }
        }
        std::vector<BufferElement> m_Elements;
        uint32_t                   m_Stride = 0;
    };

    class VertexBuffer {
    public:
        VertexBuffer(const float* data, uint32_t size);
        ~VertexBuffer();

        void Bind()   const;
        void Unbind() const;

        void                SetLayout(const BufferLayout& l) { m_Layout = l; }
        const BufferLayout& GetLayout()                const { return m_Layout; }

    private:
        uint32_t     m_RendererID;
        BufferLayout m_Layout;
    };

    class IndexBuffer {
    public:
        IndexBuffer(const uint32_t* data, uint32_t count);
        ~IndexBuffer();

        void     Bind()     const;
        void     Unbind()   const;
        uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

} // namespace Titan
