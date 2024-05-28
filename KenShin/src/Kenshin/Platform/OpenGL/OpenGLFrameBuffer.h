#pragma once
#include "Kenshin/Renderer/FrameBuffer.h"

namespace Kenshin {

	class OpenGLFrameBuffer: public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();		
		virtual void Invalicate() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetColorAttachmentID() const override;
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthStencilAttachment = 0;
		FrameBufferSpecification m_Spec;
	};
}