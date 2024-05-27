#pragma once
#include "Kenshin/Renderer/Texture.h"

namespace Kenshin {
	class KS_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const char* path);
		virtual ~OpenGLTexture2D();
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void UnBind(uint32_t slot) const override;
	private:
		uint32_t m_RendererID;
		const char* m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}