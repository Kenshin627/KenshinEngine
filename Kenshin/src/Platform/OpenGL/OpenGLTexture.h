#pragma once
#include "Kenshin/Renderer/Texture.h"

namespace Kenshin
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(uint32_t width, uint32_t height);
		OpenGLTexture(const std::string& path);
		~OpenGLTexture();
		virtual void Bind(unsigned slot) const override;
		virtual void UnBind(unsigned slot) const override;
		virtual unsigned GetWidth() const override { return m_Width; }
		virtual unsigned GetHeight() const override { return m_Height; }
		virtual void SetData(void* data, uint32_t size) const override;
	private:
		std::string m_Path;
		unsigned m_RendererID;
		unsigned m_Width;
		unsigned m_Height;
		unsigned m_InternalFormat;
		unsigned m_DataFormat;
	};
}