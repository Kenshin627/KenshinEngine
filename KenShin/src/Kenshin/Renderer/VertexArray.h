#pragma once
#include "Kenshin/Core.h"
#include "Buffer.h"

namespace Kenshin {
	class KS_API VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		static VertexArray* Create();
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vbo) = 0;
		virtual void SetIndeBuffer(const Ref<IndexBuffer>& ibo) = 0;
	};
}