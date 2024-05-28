#include "kspch.h"
#include "Renderer.h"

namespace Kenshin {
	
	Renderer::RendererData* Renderer::m_RenderData = new Renderer::RendererData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0.0f, 0.0f, width, height);
	}

	void Renderer::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		m_RenderData->Camera = camera;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<Texture2D>& texture)
	{
		vertexArray->Bind();
		shader->Bind();
		texture->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_RenderData->Camera->ViewProjectionMatrix());
		shader->UploadUniformInt("u_Texture", 0);
		RenderCommand::DrawIndexed(vertexArray);
	}
}