#pragma once
#include "Kenshin/Core/Core.h"

namespace Kenshin
{
	enum class KENSHIN_API ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2, Mat3, Mat4, Bool
	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned Size;
		unsigned Offset;
		bool normalized;
		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false) :Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), normalized(normalized)
		{
		}

		unsigned GetComponent() const 
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Mat2:    return 2 * 2;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Bool:    return 1;
			}
			KS_CORE_ASSERT(false, "unknown ShaderDataType!");
			return 0;
		}

		unsigned GetShaderDataTypeSize(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;

			case ShaderDataType::Int:    return 4;
			case ShaderDataType::Int2:   return 4 * 2;
			case ShaderDataType::Int3:   return 4 * 3;
			case ShaderDataType::Int4:   return 4 * 4;

			case ShaderDataType::Mat2:   return 4 * 2 * 2;
			case ShaderDataType::Mat3:   return 4 * 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4 * 4;

			case ShaderDataType::Bool:   return 1;
			}
			KS_CORE_ASSERT(false, "unknown ShaderDatatype!");
			return 0;
		}
	};

	class KENSHIN_API VertexBufferLayout
	{
	public:
		VertexBufferLayout():m_Stride(0) {};
		VertexBufferLayout(std::initializer_list<BufferElement> elements):m_Stride(0), m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}
		inline const std::vector<BufferElement>& GetLayout() const { return m_Elements; };
		inline const unsigned GetStride() const { return m_Stride; };
		std::vector<BufferElement>::iterator begin()
		{
			return m_Elements.begin();
		}
		std::vector<BufferElement>::iterator end()
		{
			return m_Elements.end();
		}
		std::vector<BufferElement>::const_iterator cbegin()
		{
			return m_Elements.cbegin();
		}
		std::vector<BufferElement>::const_iterator cend()
		{
			return m_Elements.cend();
		}
		~VertexBufferLayout() {};
	private:
		void CalculateOffsetAndStride()
		{
			unsigned index = 0;
			unsigned offset = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		unsigned m_Stride;
	};

	class KENSHIN_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() = 0;
		virtual void unBind() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetLayout() const = 0;
		static Ref<VertexBuffer> Create(const float* vertices, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size);
	};

	class KENSHIN_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() = 0;
		virtual void unBind() = 0;
		virtual unsigned GetCount() const = 0;
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};
}