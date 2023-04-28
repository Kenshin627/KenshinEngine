#include "kspch.h"
#include "OpenGLFrameBuffer.h"
#include <glad/gl.h>

namespace Kenshin
{
	static const uint32_t s_MaxFrambufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			} 
			else 
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else 
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisample), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			if (format == FrameBufferTextureFormat::DEPTH24STENCIL8)
			{
				return true;
			}
			return false;
		}

		static GLenum FBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case Kenshin::FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
				break;
			case Kenshin::FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
				break;
			}
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec): m_Specification(spec), m_RendererID(0)
	{		
		for (auto& attachment : spec.attachments.attachments)
		{
			if (!Utils::IsDepthFormat(attachment.TextureFormat))
			{
				m_ColorAttachmentSpecifications.emplace_back(attachment);
			}
			else {
				m_DpethAttachmentSpecification = attachment;
			}
		}
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		DeleteFrameBuffer();
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::DeleteFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			DeleteFrameBuffer();
		}
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (m_DpethAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DpethAttachmentSpecification.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height); break;
			}
		}
		
		if (m_ColorAttachments.size() > 1)
		{
			KS_CORE_ASSERT(m_ColorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		KS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer create Failed!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			KS_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}	

	uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(uint32_t attachmentIndex) const
	{
		KS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "out of vector bounds!");
		return m_ColorAttachments[attachmentIndex];
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		KS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		KS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "");
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}