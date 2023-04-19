#include "kspch.h"
#include "SubTexture2D.h"

namespace Kenshin
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max):m_Texture(texture)
	{
		m_Coords[0] = { min.x, min.y };
		m_Coords[1] = { max.x, min.y };
		m_Coords[2] = { max.x, max.y };
		m_Coords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spiritSize)
	{
		unsigned texWidth  = texture->GetWidth();
		unsigned texHeight = texture->GetHeight();

		glm::vec2 min = { coords.x * cellSize.x / texWidth, (coords.y * cellSize.y) / texHeight };
		glm::vec2 max = { ((coords.x + spiritSize.x) * cellSize.x)/ texWidth, ((coords.y  + spiritSize.y) * cellSize.y) / texHeight };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}