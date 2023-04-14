#pragma once

namespace Kenshin
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() = 0;
		virtual void unBind() = 0;
		static VertexBuffer* CreateBuffer(const float* vertices, unsigned size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() = 0;
		virtual void unBind() = 0;
		virtual unsigned GetCount() const = 0;
		static IndexBuffer* CreateBuffer(const unsigned* indices, unsigned count);
	};


}