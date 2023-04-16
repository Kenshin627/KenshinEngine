#include "kspch.h"
#include "OpenGLTexture.h"
#include <glad/gl.h>
#include "stb_image.h"

namespace Kenshin
{
	OpenGLTexture::OpenGLTexture(const std::string& path):m_Path(path
	), m_RendererID(0), m_Width(0), m_Height(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		stbi_set_flip_vertically_on_load(true);
		int width, height, channel;
		unsigned internalFormat = 0, dataFormat = 0;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel, 0);
		KS_CORE_ASSERT("Failed to load Texture:{0}", path);
		m_Width = width;
		m_Height = height;
		if (channel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		KS_CORE_ASSERT(internalFormat && dataFormat, "image type not supported!");
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
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

	void OpenGLTexture::UnBind() const
	{
	
	}
}