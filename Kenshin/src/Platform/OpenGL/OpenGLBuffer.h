#pragma once
#include "Kenshin/Renderer/Buffer.h"

namespace Kenshin
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* vertices, unsigned size);
		virtual ~OpenGLVertexBuffer();
		virtual void Bind() override;
		virtual void unBind() override;
		virtual void SetLayout(const VertexBufferLayout& layout) override;
		virtual const VertexBufferLayout& GetLayout() const override;

	private:
		unsigned m_RendererID;
		VertexBufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const unsigned* indices, unsigned count);
		virtual ~OpenGLIndexBuffer();
		virtual void Bind() override;
		virtual void unBind() override;
		virtual unsigned GetCount() const override { return m_Count; };
	private:
		unsigned m_RendererID;
		unsigned m_Count;
	};
}