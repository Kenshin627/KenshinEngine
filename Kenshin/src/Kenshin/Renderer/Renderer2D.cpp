#include "kspch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Kenshin
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DStorageData
	{
		const unsigned MaxQuadCount = 10000;
		const unsigned MaxVertices = MaxQuadCount * 4;
		const unsigned MaxIndices = MaxQuadCount * 6;
		Ref<VertexArray> QuadVA;	
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		unsigned QuadIndexedCount = 0;
		QuadVertex* QuadVertexArrayBufferBase = nullptr;
		QuadVertex* QuadVertexArrayBufferPtr = nullptr;
	};

	static Renderer2DStorageData s_Data;

	void Renderer2D::Init()
	{		
		#pragma region QUAD
		s_Data.QuadVA = Kenshin::VertexArray::CreateVertexArray();
		
		s_Data.QuadVB = Kenshin::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		Kenshin::VertexBufferLayout quadLayout =
		{
			{ "aPosition", Kenshin::ShaderDataType::Float3 },
			{ "aColor", Kenshin::ShaderDataType::Float4 },
			{ "aTexCoord", Kenshin::ShaderDataType::Float2 }
		};
		s_Data.QuadVB->SetLayout(quadLayout);
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);
		
		//Indices
		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		unsigned offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+=6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}
		Kenshin::Ref<Kenshin::IndexBuffer> quadEBO = Kenshin::IndexBuffer::Create(indices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadEBO);
		delete[] indices;
		#pragma endregion

		#pragma region SHADER		
		s_Data.TextureShader = Shader::Create("resource/shaders/texture.glsl");
		#pragma endregion				 

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, sizeof(uint32_t));
	}
	void Renderer2D::ShutDown()
	{
		
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.QuadVA->Bind();
		s_Data.TextureShader->Bind();
		//s_Data.TextureShader->SetInt("sampler", 0);
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		s_Data.QuadIndexedCount = 0;
		s_Data.QuadVertexArrayBufferBase = new QuadVertex[s_Data.MaxVertices];
		s_Data.QuadVertexArrayBufferPtr = s_Data.QuadVertexArrayBufferBase;

	}
	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		uint32_t size = (uint8_t*)s_Data.QuadVertexArrayBufferPtr - (uint8_t*)s_Data.QuadVertexArrayBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexArrayBufferBase, size);
		RendererCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexedCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		float half_W = size.x / 2.0f;
		float half_H = size.y / 2.0f;
		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadIndexedCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0f));
		auto translate = glm::translate(glm::mat4(1.0), position);

		texture->Bind();
		s_Data.TextureShader->SetVec4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetMat4("u_ModelMatrix", translate * scale);
		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
		texture->UnBind();
	}

	void Renderer2D::DrawRorateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRorateQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRorateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0f));
		auto translate = glm::translate(glm::mat4(1.0), position);
		auto rotate = glm::rotate(glm::mat4(1.0), rotation, { 0, 0, 1 });
		s_Data.WhiteTexture->Bind();
		s_Data.TextureShader->SetVec4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_ModelMatrix", translate * rotate * scale);
		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
		s_Data.WhiteTexture->UnBind();
	}

	void Renderer2D::DrawRorateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRorateQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRorateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		auto scale = glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0f));
		auto translate = glm::translate(glm::mat4(1.0), position);
		auto rotate = glm::rotate(glm::mat4(1.0f), rotation, { 0, 0, 1 });
		texture->Bind();
		s_Data.TextureShader->SetVec4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetMat4("u_ModelMatrix", translate * rotate * scale);
		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
		texture->UnBind();
	}
}