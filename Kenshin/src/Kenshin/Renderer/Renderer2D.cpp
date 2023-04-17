#include "kspch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Kenshin
{
	struct Renderer2DStorageData
	{
		Ref<VertexArray> QuadVA;		
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorageData* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorageData;

		#pragma region QUAD
		s_Data->QuadVA = Kenshin::VertexArray::CreateVertexArray();
		float quadVertices[4 * 5] = {
			-0.5,  0.5, 0.0, 0.0, 1.0,
			 0.5,  0.5, 0.0, 1.0, 1.0,
			-0.5, -0.5, 0.0, 0.0, 0.0,
			 0.5, -0.5, 0.0, 1.0, 0.0
		};
		unsigned quadIndices[6] = { 0, 1, 2, 1, 2, 3 };
		Kenshin::Ref<Kenshin::VertexBuffer> quadVBO = Kenshin::Ref<Kenshin::VertexBuffer>(Kenshin::VertexBuffer::CreateBuffer(quadVertices, sizeof(quadVertices)));
		Kenshin::VertexBufferLayout quadLayout =
		{
			{ "aPosition", Kenshin::ShaderDataType::Float3 },
			{ "aTexCoord", Kenshin::ShaderDataType::Float2 }
		};
		quadVBO->SetLayout(quadLayout);
		s_Data->QuadVA->AddVertexBuffer(quadVBO);
		Kenshin::Ref<Kenshin::IndexBuffer> quadEBO = Kenshin::Ref<Kenshin::IndexBuffer>(Kenshin::IndexBuffer::CreateBuffer(quadIndices, 6));
		s_Data->QuadVA->SetIndexBuffer(quadEBO);
		#pragma endregion

		#pragma region SHADER		
		s_Data->TextureShader = Shader::Create("resource/shaders/texture.glsl");
		#pragma endregion				 

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data->WhiteTexture->SetData(&data, sizeof(uint32_t));
	}
	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("sampler", 0);
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::EndScene()
	{

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0f));
		auto translate = glm::translate(glm::mat4(1.0), position);
		s_Data->WhiteTexture->Bind();
		s_Data->TextureShader->SetVec4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_ModelMatrix", translate * scale);
		s_Data->QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVA);
		s_Data->WhiteTexture->UnBind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0f));
		auto translate = glm::translate(glm::mat4(1.0), position);

		texture->Bind();
		s_Data->TextureShader->SetVec4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->SetMat4("u_ModelMatrix", translate * scale);
		s_Data->QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVA);
		texture->UnBind();
	}
}