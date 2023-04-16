#pragma once
#include "VertexArray.h"
#include "RendererCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Kenshin
{
	class KENSHIN_API Renderer
	{
	public:
		static void Init();
		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform);
	private:
		static glm::mat4 m_ViewProjectionMatrix;
	};
}