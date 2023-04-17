#include "kspch.h"
#include "OpenGLTexture.h"
#include <glad/gl.h>
#include "stb_image.h"

namespace Kenshin
{
	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height):m_Width(width), m_Height(height),m_InternalFormat(0), m_DataFormat(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		m_InternalFormat = GL_RGBA8, m_DataFormat = GL_RGBA;

		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size) const
	{
		size_t bpp = m_InternalFormat == GL_RGBA8 ? 4 : 3;
		KS_CORE_ASSERT(bpp * m_Width * m_Height == size, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::OpenGLTexture(const std::string& path):m_Path(path
	), m_RendererID(0), m_Width(0), m_Height(0), m_InternalFormat(0), m_DataFormat(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		stbi_set_flip_vertically_on_load(true);
		int width, height, channel;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel, 0);
		KS_CORE_ASSERT("Failed to load Texture:{0}", path);
		m_Width = width;
		m_Height = height;
		if (channel == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		KS_CORE_ASSERT(m_InternalFormat && m_DataFormat, "image type not supported!");
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind(unsigned slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture::UnBind(unsigned slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}