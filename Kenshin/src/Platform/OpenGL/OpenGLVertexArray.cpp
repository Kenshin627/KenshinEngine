#include "kspch.h"
#include "OpenGLVertexArray.h"
#include <glad/gl.h>

namespace Kenshin
{
	static unsigned GetOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		    return GL_FLOAT;
			case ShaderDataType::Float2:		return GL_FLOAT;
			case ShaderDataType::Float3:		return GL_FLOAT;
			case ShaderDataType::Float4:		return GL_FLOAT;
			case ShaderDataType::Int:			return GL_INT;
			case ShaderDataType::Int2:			return GL_INT;
			case ShaderDataType::Int3:			return GL_INT;
			case ShaderDataType::Int4:			return GL_INT;
			case ShaderDataType::Mat2:			return GL_FLOAT;
			case ShaderDataType::Mat3:			return GL_FLOAT;
			case ShaderDataType::Mat4:			return GL_FLOAT;
			case ShaderDataType::Bool:			return GL_BOOL;
		}
		KS_CORE_ASSERT(false, "unknown ShaderDataType!");
		return 0;
	}
	OpenGLVertexArray::OpenGLVertexArray():m_RendererID(0) 
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

	void OpenGLVertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		unsigned index = 0;
		auto layout = vertexBuffer->GetLayout();
		for (auto& element : layout)
		{			
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponent(), GetOpenGLDataType(element.Type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset); 
				index++;
				break;
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, element.GetComponent(), GetOpenGLDataType(element.Type), layout.GetStride(), (const void*)element.Offset); 
				index++;
				break;
			case ShaderDataType::Mat2:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				uint8_t count = element.GetComponent();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index, count, GetOpenGLDataType(element.Type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffer() const
	{
		return m_VertexBuffers;
	}
	const Ref<IndexBuffer> OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	};
}