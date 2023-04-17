#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Renderer/OrthographicCamera.h"
#include "Kenshin/Renderer/VertexArray.h"
#include "Kenshin/Renderer/OrthographicCamera.h"
#include "Kenshin/Renderer/Shader.h"

namespace Kenshin
{
	class KENSHIN_API Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3 color);
	};
}