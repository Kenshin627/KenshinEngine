#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Kenshin {
	class Renderer
	{
	public:
		struct RendererData
		{
			std::shared_ptr<OrthographicCamera> Camera;
		};
	public:
		static void BeginScene(const std::shared_ptr<OrthographicCamera>& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader);
	public:
		static RendererAPI::API GetAPI() { return RenderCommand::GetAPI(); }
	private:
		static RendererData* m_RenderData;
	};
}