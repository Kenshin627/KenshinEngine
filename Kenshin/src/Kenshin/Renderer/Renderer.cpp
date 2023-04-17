#include "kspch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Kenshin
{
	glm::mat4 Renderer::m_ViewProjectionMatrix = glm::mat4(1.0);
	void Renderer::Init()
	{
		RendererCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() { }

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_ViewProjectionMatrix);
		shader->SetMat4("u_ModelMatrix", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(size_t width, size_t height)
	{
		RendererCommand::ResizeViewport(0, 0, width, height);
	}
}