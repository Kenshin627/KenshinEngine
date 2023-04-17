#include "kspch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Kenshin
{
	struct Renderer2DStorageData
	{
		Ref<VertexArray> QuadVA;
		Ref<Shader> Shader;
	};

	static Renderer2DStorageData* s_Data;

	void Renderer2D::Init()
	{		
		s_Data = new Renderer2DStorageData;
		
		#pragma region QUAD
		s_Data->QuadVA = Kenshin::VertexArray::CreateVertexArray();
		float quadVertices[4 * 3] = {
			-0.5,  0.5, 0.0,
			 0.5,  0.5, 0.0,
			-0.5, -0.5, 0.0,
			 0.5, -0.5, 0.0
		};
		unsigned quadIndices[6] = { 0, 1, 2, 1, 2, 3 };
		Kenshin::Ref<Kenshin::VertexBuffer> quadVBO = Kenshin::Ref<Kenshin::VertexBuffer>(Kenshin::VertexBuffer::CreateBuffer(quadVertices, sizeof(quadVertices)));
		Kenshin::VertexBufferLayout quadLayout =
		{
			{ "aPosition", Kenshin::ShaderDataType::Float3 }
		};
		quadVBO->SetLayout(quadLayout);
		s_Data->QuadVA->AddVertexBuffer(quadVBO);
		Kenshin::Ref<Kenshin::IndexBuffer> quadEBO = Kenshin::Ref<Kenshin::IndexBuffer>(Kenshin::IndexBuffer::CreateBuffer(quadIndices, 6));
		s_Data->QuadVA->SetIndexBuffer(quadEBO);
		#pragma endregion

		#pragma region SHADER
		s_Data->Shader = Shader::Create("resource/shaders/flatColor.glsl");
		#pragma endregion				 
	}					 
	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera) 
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::EndScene()
	{
		
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3 color)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 0.0f));
		auto translate = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, 0.0f));
		s_Data->Shader->SetVec3("u_Color", color);
		
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ModelMatrix", translate * scale);
		s_Data->QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVA);
	}
}