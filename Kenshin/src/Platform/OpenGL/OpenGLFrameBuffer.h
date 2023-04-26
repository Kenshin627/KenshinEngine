#pragma once
#include "Kenshin/Renderer/FrameBuffer.h"

namespace Kenshin
{
	class  OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual~OpenGLFrameBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t GetColorAttachment() const override;
		virtual const FrameBufferSpecification& GetSpecification() const { return m_Specification; }
	private:
		void Invalidate();
	private:
		FrameBufferSpecification m_Specification;
		uint32_t m_RendererID;
		uint32_t m_ColorAttachmentID;
		uint32_t m_DepthAttachmentID;
	};
}