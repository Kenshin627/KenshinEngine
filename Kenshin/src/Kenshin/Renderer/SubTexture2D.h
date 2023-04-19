#pragma once
#include "Texture.h"
#include "Kenshin/Core/Core.h"
#include <glm.hpp>

namespace Kenshin
{
	class KENSHIN_API SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spiritSize = { 1.0f, 1.0f });
		const Ref<Texture2D>& GetTexture() const { return m_Texture; };
		const glm::vec2* GetCoords() const { return m_Coords; };
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_Coords[4];
	};
}