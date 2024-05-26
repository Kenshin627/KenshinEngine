#pragma once
#include "Kenshin/Core.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Kenshin {
	class KS_API Renderer
	{
	public:
		struct RendererData
		{
			Ref<OrthographicCamera> Camera;
		};
	public:
		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, Ref<Shader>& shader);
	public:
		static RendererAPI::API GetAPI() { return RenderCommand::GetAPI(); }
	private:
		static RendererData* m_RenderData;
	};
}