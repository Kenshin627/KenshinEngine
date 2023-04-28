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
		virtual void DeleteFrameBuffer() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t attachmentIndex) const override;
		virtual const FrameBufferSpecification& GetSpecification() const { return m_Specification; }
	private:
		void Invalidate();
	private:
		FrameBufferSpecification m_Specification;
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DpethAttachmentSpecification = FrameBufferTextureFormat::None;
		uint32_t m_RendererID;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}