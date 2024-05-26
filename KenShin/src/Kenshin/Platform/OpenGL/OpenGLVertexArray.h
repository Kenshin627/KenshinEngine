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
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vbo) override;
		virtual void SetIndeBuffer(const Ref<IndexBuffer>& ibo) override;
	private:
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}