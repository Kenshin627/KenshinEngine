#pragma once

namespace Kenshin
{
	class KENSHIN_API Texture
	{
	public:
		virtual unsigned GetWidth() const = 0;
		virtual unsigned GetHeight() const = 0;
		virtual void Bind(unsigned slot = 0) const = 0;
		virtual void UnBind() const = 0;
	};

	class KENSHIN_API Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(const std::string& path);
	};
}