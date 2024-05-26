#include "kspch.h"
#include "Renderer.h"

namespace Kenshin {
	
	Renderer::RendererData* Renderer::m_RenderData = new Renderer::RendererData;
	void Renderer::BeginScene(const std::shared_ptr<OrthographicCamera>& camera)
	{
		m_RenderData->Camera = camera;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_RenderData->Camera->ViewProjectionMatrix());
		RenderCommand::DrawIndexed(vertexArray);
	}
}