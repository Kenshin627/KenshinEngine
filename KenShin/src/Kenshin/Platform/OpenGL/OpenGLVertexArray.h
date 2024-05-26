#pragma once
#include "Kenshin/Renderer/VertexArray.h"

namespace Kenshin {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) override;
		virtual void SetIndeBuffer(const std::shared_ptr<IndexBuffer>& ibo) override;
	private:
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}