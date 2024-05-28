#pragma once
#include "Kenshin/Core.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Kenshin {
	class KS_API Renderer
	{
	public:
		struct RendererData
		{
			Ref<OrthographicCamera> Camera;
		};
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<Texture2D>& texture);
	public:
		static RendererAPI::API GetAPI() { return RenderCommand::GetAPI(); }
	private:
		static RendererData* m_RenderData;
	};
}