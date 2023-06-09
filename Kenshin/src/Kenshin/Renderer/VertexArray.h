#pragma once
#include "Buffer.h"
#include "Kenshin/Core/Core.h"

namespace Kenshin
{
	class  VertexArray
	{
	public:
		VertexArray() {};
		virtual~VertexArray(){};
		virtual void Bind() const = 0;
		virtual void unBind() const = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;
		static Ref<VertexArray> CreateVertexArray();
	};
}