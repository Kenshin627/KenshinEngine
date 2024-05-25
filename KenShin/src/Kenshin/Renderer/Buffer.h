#pragma once
#include "Kenshin/Core.h"
#include "Kenshin/Log.h"
#include <cstdint>

typedef unsigned int GLenum;
#define GL_INT 0x1404
#define GL_FLOAT 0x1406
#define GL_BOOL 0x8B56

namespace Kenshin {

	enum class ShaderDataType
	{
		None = 0,
		Int, Int2, Int3, Int4, 
		Float, Float2, Float3, Float4,
		Mat2, Mat3, Mat4,
		Bool
	};

	static uint32_t CalcShaderDataTpeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat2:		return 4 * 2 * 2;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}
		KS_ASSET(false, "unknown shaderDataType!");
		return 0;
	}

	static uint32_t CalcShaderDataTypeComponent(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat2:		return 2 * 2;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}
		KS_ASSET(false, "unknown shaderDataType!");
		return 0;
	}

	static GLenum ConvertShaderDataTypeToGLType(ShaderDataType type)
	{
		switch (type)
		{
			case Kenshin::ShaderDataType::Int:		return GL_INT;
			case Kenshin::ShaderDataType::Int2:		return GL_INT;
			case Kenshin::ShaderDataType::Int3:		return GL_INT;
			case Kenshin::ShaderDataType::Int4:		return GL_INT;
			case Kenshin::ShaderDataType::Float:	return GL_FLOAT;
			case Kenshin::ShaderDataType::Float2:	return GL_FLOAT;
			case Kenshin::ShaderDataType::Float3:	return GL_FLOAT;
			case Kenshin::ShaderDataType::Float4:	return GL_FLOAT;
			case Kenshin::ShaderDataType::Mat2:		return GL_FLOAT;
			case Kenshin::ShaderDataType::Mat3:		return GL_FLOAT;
			case Kenshin::ShaderDataType::Mat4:		return GL_FLOAT;
			case Kenshin::ShaderDataType::Bool:		return GL_BOOL;
		}
		KS_ASSET(false, "unknown shaderDataType!");
		return 0;
	}

	struct LayoutElement
	{
		std::string Name = "";
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		uint32_t ComponentCount = 0;
		bool Normalized = false;
		GLenum GlType = GL_INT;
		LayoutElement() {}
		LayoutElement(const std::string& name, ShaderDataType type, bool normalized = false):
			Name(name),
			Type(type),
			Size(CalcShaderDataTpeSize(type)),
			Offset(0),
			ComponentCount(CalcShaderDataTypeComponent(type)),
			Normalized(normalized),
			GlType(ConvertShaderDataTypeToGLType(type))
		{ }
	};

	class BufferLayout
	{
	public:
		BufferLayout() :m_Elements({}), m_Stride(0) {};
		BufferLayout(const std::initializer_list<LayoutElement>& elements) : m_Elements(elements), m_Stride(0)
		{
			uint32_t index  = 0;
			uint32_t offset = 0;
			for (auto& element : m_Elements)
			{				
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
				index++;
			}
		}
		const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
		uint32_t GetStride()                   const { return m_Stride; }
		std::vector<LayoutElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<LayoutElement>::iterator end()   { return m_Elements.end(); }
		std::vector<LayoutElement>::const_iterator cbegin() { return m_Elements.cbegin(); }
		std::vector<LayoutElement>::const_iterator cend() { return m_Elements.cend(); }
	private:
		std::vector<LayoutElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void SetLayout(BufferLayout& layout) = 0;
		virtual BufferLayout& GetLayout() = 0;
		static VertexBuffer* Create(float* data, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual uint32_t GetCount() const = 0;
		static IndexBuffer* Create(uint32_t* data, uint32_t count);
	};

}