#include "kspch.h"
#include "OpenGLTexture.h"
#include "Kenshin/Core.h"
#include "Kenshin/Log.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace Kenshin {
	OpenGLTexture2D::OpenGLTexture2D(const char* path) :m_Path(path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, chanel;
		stbi_uc* data = stbi_load(path, &width, &height, &chanel, 0);
		KS_ASSET(data, "load image failed!");
		m_Width = width;
		m_Height = height;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	uint32_t OpenGLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::UnBind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}