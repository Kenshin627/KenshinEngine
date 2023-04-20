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
		static const unsigned MaxQuadCount = 200000;
		static const unsigned MaxVertices = MaxQuadCount * 4;
		static const unsigned MaxIndices = MaxQuadCount * 6;
		static const unsigned VerticeCount = 4;
		Ref<VertexArray> QuadVA;	
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> QuadShader;
		unsigned QuadIndexedCount = 0;
		QuadVertex* QuadVertexArrayBufferBase = nullptr;
		QuadVertex* QuadVertexArrayBufferPtr = nullptr;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned TextureSlotIndex = 1;
		glm::vec4 QuadPosition[4];
		glm::vec2 QuadTexCoord[4];
		Renderer2D::Statistics Stats;
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

		s_Data.QuadPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadTexCoord[0] = { 0.0f, 0.0f };
		s_Data.QuadTexCoord[1] = { 1.0f, 0.0f };
		s_Data.QuadTexCoord[2] = { 1.0f, 1.0f };
		s_Data.QuadTexCoord[3] = { 0.0f, 1.0f };
		#pragma endregion

		#pragma region SHADER		
		s_Data.QuadShader = Shader::Create("resource/shaders/quad.glsl");
		#pragma endregion				 

		#pragma region Texture
		auto whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(uint32_t));
		s_Data.TextureSlots[0] = whiteTexture;
		#pragma endregion
	}

	void Renderer2D::ShutDown() { }

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{		
		s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		
		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexedCount)
		{
			uint32_t size = (uint8_t*)s_Data.QuadVertexArrayBufferPtr - (uint8_t*)s_Data.QuadVertexArrayBufferBase;
			s_Data.QuadVB->SetData(s_Data.QuadVertexArrayBufferBase, size);
			for (size_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			s_Data.QuadShader->Bind();
			RendererCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexedCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });		
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, rotation, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}
	
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//TODO:检查maxIndices，如果到达上限，进入下一次Batch
		if (s_Data.QuadIndexedCount >= s_Data.MaxIndices)
		{
			NextBatch();
		}
		for (size_t i = 0; i < s_Data.VerticeCount; i++)
		{
			s_Data.QuadVertexArrayBufferPtr->Position = glm::vec3(transform * s_Data.QuadPosition[i]);
			s_Data.QuadVertexArrayBufferPtr->Color = color;
			s_Data.QuadVertexArrayBufferPtr->TexCoord = s_Data.QuadTexCoord[i];
			s_Data.QuadVertexArrayBufferPtr->TilingFactor = 1;
			s_Data.QuadVertexArrayBufferPtr->TexIndex = 0; //whiteTexture
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		if (s_Data.QuadIndexedCount >= s_Data.MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;
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
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
			{
				NextBatch();
			}
			else {
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}			
		}

		for (size_t i = 0; i < s_Data.VerticeCount; i++)
		{
			s_Data.QuadVertexArrayBufferPtr->Position = glm::vec3(transform * s_Data.QuadPosition[i]);
			s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
			s_Data.QuadVertexArrayBufferPtr->TexCoord = s_Data.QuadTexCoord[i];
			s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		if (s_Data.QuadIndexedCount >= s_Data.MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *subTexture->GetTexture())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
			{
				NextBatch();
			}
			else {
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = subTexture->GetTexture();
				s_Data.TextureSlotIndex++;
			}
		}

		auto texCoords = subTexture->GetCoords();
		for (size_t i = 0; i < s_Data.VerticeCount; i++)
		{
			s_Data.QuadVertexArrayBufferPtr->Position = glm::vec3(transform * s_Data.QuadPosition[i]);
			s_Data.QuadVertexArrayBufferPtr->Color = tintColor;
			s_Data.QuadVertexArrayBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexArrayBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexArrayBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	const Renderer2D::Statistics& Renderer2D::GetStatistics()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStatistics()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexedCount = 0;
		s_Data.QuadVertexArrayBufferPtr = s_Data.QuadVertexArrayBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
}