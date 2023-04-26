#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Renderer/OrthographicCamera.h"
#include "Kenshin/Renderer/VertexArray.h"
#include "Kenshin/Renderer/OrthographicCamera.h"
#include "Kenshin/Renderer/Shader.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Camera.h"

namespace Kenshin
{
	struct RenderStats;
	class  Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0));
		static void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor);
		static void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount()  const { return QuadCount * 6; }
		};
		static const Statistics& GetStatistics();
		static void ResetStatistics();
	private:
		static void StartBatch();
		static void NextBatch();
	};
}