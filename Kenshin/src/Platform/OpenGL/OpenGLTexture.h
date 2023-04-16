#pragma once
#include "Kenshin/Renderer/Texture.h"

namespace Kenshin
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path);
		~OpenGLTexture();
		virtual void Bind(unsigned slot) const override;
		virtual void UnBind() const override;
		virtual unsigned GetWidth() const override { return m_Width; }
		virtual unsigned GetHeight() const override { return m_Height; }
	private:
		std::string m_Path;
		unsigned m_RendererID;
		unsigned m_Width;
		unsigned m_Height;
	};
}