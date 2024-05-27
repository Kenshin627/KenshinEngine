#pragma once
#include "Kenshin/Core.h"
#include <cstdint>

namespace Kenshin {
	class KS_API Texture2D
	{
	public:
		Texture2D() {};
		virtual ~Texture2D() {};
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void UnBind(uint32_t slot) const = 0;
		static Ref<Texture2D> Create(const char* path);
	};
}