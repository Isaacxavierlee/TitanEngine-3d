#pragma once
#include "Buffer.h"
#include <memory>
#include <vector>

namespace Titan {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void Bind()   const;
        void Unbind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb);
        void SetIndexBuffer (const std::shared_ptr<IndexBuffer>&  ib);

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VBOs; }
        const std::shared_ptr<IndexBuffer>&               GetIndexBuffer()   const { return m_IBO; }

    private:
        uint32_t m_RendererID;
        uint32_t m_AttribIndex = 0;
        std::vector<std::shared_ptr<VertexBuffer>> m_VBOs;
        std::shared_ptr<IndexBuffer>               m_IBO;
    };

} // namespace Titan
