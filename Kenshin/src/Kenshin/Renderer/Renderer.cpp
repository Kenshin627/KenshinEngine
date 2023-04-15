#include "kspch.h"
#include "Renderer.h"

namespace Kenshin
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}