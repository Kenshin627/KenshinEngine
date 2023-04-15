#include "kspch.h"
#include "Renderer.h"

namespace Kenshin
{
	glm::mat4 Renderer::m_ViewProjectionMatrix = glm::mat4(1.0);
	void Renderer::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_ViewProjectionMatrix);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}