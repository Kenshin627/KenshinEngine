#pragma once
#include "Buffer.h"

namespace Kenshin {
	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		static VertexArray* Create();
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) = 0;
		virtual void SetIndeBuffer(const std::shared_ptr<IndexBuffer>& ibo) = 0;
	};
}