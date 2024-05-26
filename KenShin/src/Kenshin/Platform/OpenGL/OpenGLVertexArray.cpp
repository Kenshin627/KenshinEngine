#include "kspch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Kenshin {
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo)
	{
		KS_ASSET(vbo->GetLayout().GetElements().size(), "Bufferlayout is empty!");
		glBindVertexArray(m_RendererID);
		vbo->Bind();
		uint32_t index = 0;
		BufferLayout& layout = vbo->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.ComponentCount, element.GlType, element.Normalized, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
		glBindVertexArray(0);
		m_VertexBuffers.push_back(vbo);
	}

	void OpenGLVertexArray::SetIndeBuffer(const std::shared_ptr<IndexBuffer>& ibo)
	{
		glBindVertexArray(m_RendererID);
		ibo->Bind();
		glBindVertexArray(0);
		m_IndexBuffer = ibo;
	}
}