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
		float TilingFactor;
		float TexIndex;
	};

	struct Renderer2DStorageData
	{
		static const unsigned MaxTextureSlots = 32;
		static const unsigned MaxQuadCount = 30000;
		static const unsigned MaxVertices = MaxQuadCount * 4;
		static const unsigned MaxIndices = MaxQuadCount * 6;
		Ref<VertexArray> QuadVA;	
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		unsigned QuadIndexedCount = 0;
		QuadVertex* QuadVertexArrayBufferBase = nullptr;
		QuadVertex* QuadVertexArrayBufferPtr = nullptr;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned TextureSlotIndex = 1;
	};

	static Renderer2DStorageData s_Data;

	void Renderer2D::Init()
	{		
		#pragma region QUAD
		s_Data.QuadVA = Kenshin::VertexArray::CreateVertexArray();
		
		s_Data.QuadVB = Kenshin::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVB->SetLayout({
			{ "aPosition", Kenshin::ShaderDataType::Float3 },
			{ "aColor", Kenshin::ShaderDataType::Float4 },
			{ "aTexCoord", Kenshin::ShaderDataType::Float2 },
			{ "aTilingFactor", Kenshin::ShaderDataType::Float },
			{ "aTexIndex", Kenshin::ShaderDataType::Float }
		});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);
		s_Data.QuadVertexArrayBufferBase = new QuadVertex[s_Data.MaxVertices];
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

		#pragma region Texture
		auto whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(uint32_t));
		s_Data.TextureSlots[0] = whiteTexture;
		#pragma endregion
	}
	void Renderer2D::ShutDown()
	{
		
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		
		s_Data.QuadIndexedCount = 0;
		s_Data.QuadVertexArrayBufferPtr = s_Data.QuadVertexArrayBufferBase;
		s_Data.TextureSlotIndex = 1;
	}
	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		uint32_t size = (uint8_t*)s_Data.QuadVertexArrayBufferPtr - (uint8_t*)s_Data.QuadVertexArrayBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexArrayBufferBase, size);
		for (size_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
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
		constexpr float tilingFactor = 1.0f;
		constexpr float TexIndex = 0.0f;
		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = TexIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = TexIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = TexIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = color;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = TexIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadIndexedCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t vertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		float half_W = size.x / 2.0f;
		float half_H = size.y / 2.0f;
		float textureIndex = 0.0f;
		//TODO: check maxIndices..

		for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			//TODO::check maxtexture Slots..
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y - half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x + half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadVertexArrayBufferPtr->Position = { position.x - half_W, position.y + half_H, 0 };
		s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
		s_Data.QuadVertexArrayBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexArrayBufferPtr++;

		s_Data.QuadIndexedCount += 6;
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
		//s_Data.WhiteTexture->Bind();
		s_Data.TextureShader->SetVec4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_ModelMatrix", translate * rotate * scale);
		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
		//s_Data.WhiteTexture->UnBind();
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