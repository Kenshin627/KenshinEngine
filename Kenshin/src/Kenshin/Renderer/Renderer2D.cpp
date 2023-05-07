#include "kspch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

#include "Kenshin/Renderer/UniformBuffer.h"



namespace Kenshin
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TilingFactor;
		float TexIndex;
		int EntityId;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thinness;
		float Fade;
		int EntityId;
	};

	struct Renderer2DStorageData
	{
		static const unsigned MaxTextureSlots = 32;
		static const unsigned MaxQuadCount = 200000;
		static const unsigned MaxVertices = MaxQuadCount * 4;
		static const unsigned MaxIndices = MaxQuadCount * 6;
		static const unsigned VerticeCount = 4;

		//Quad
		Ref<VertexArray> QuadVA;	
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> QuadShader;
		unsigned QuadIndexedCount = 0;
		QuadVertex* QuadVertexArrayBufferBase = nullptr;
		QuadVertex* QuadVertexArrayBufferPtr  = nullptr;

		//Circle
		Ref<VertexArray> CircleVA;
		Ref<VertexBuffer> CircleVB;
		Ref<Shader> CircleShader;
		unsigned CirlceIndexedCount = 0;
		CircleVertex* CircleVertexArrayBufferBase = nullptr;
		CircleVertex* CircleVertexArrayBufferPtr  = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned TextureSlotIndex = 1;
		glm::vec4 QuadPosition[4];
		glm::vec2 QuadTexCoord[4];
		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DStorageData s_Data;

	void Renderer2D::Init()
	{		
		#pragma region QUAD
		s_Data.QuadVA = VertexArray::CreateVertexArray();
		
		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVB->SetLayout({
			{ "a_Position",     ShaderDataType::Float3 },
			{ "a_Color",        ShaderDataType::Float4 },
			{ "a_TexCoord",     ShaderDataType::Float2 },
			{ "a_TilingFactor", ShaderDataType::Float },
			{ "a_TexIndex",     ShaderDataType::Float },
			{ "a_EntityId",     ShaderDataType::Int }
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
		//Kenshin::Ref<Kenshin::IndexBuffer> circleEBO = Kenshin::IndexBuffer::Create(indices, s_Data.MaxIndices);
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

		#pragma region CIRCLE
		s_Data.CircleVA = VertexArray::CreateVertexArray();
		s_Data.CircleVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVB->SetLayout({
			{ "a_WorldPosition", ShaderDataType::Float3 },
			{ "a_LocalPosition", ShaderDataType::Float3 },
			{ "a_Color",         ShaderDataType::Float4 },
			{ "a_Thinness",      ShaderDataType::Float },
			{ "a_Fade",          ShaderDataType::Float },
			{ "a_EntityId",      ShaderDataType::Int },
		});
		s_Data.CircleVA->AddVertexBuffer(s_Data.CircleVB);
		s_Data.CircleVertexArrayBufferBase = new CircleVertex[s_Data.MaxVertices];
		s_Data.CircleVA->SetIndexBuffer(quadEBO);
		
		#pragma endregion

		#pragma region SHADER		
		s_Data.QuadShader   = Shader::Create("resource/shaders/renderer2D_quad.glsl");

		s_Data.CircleShader = Shader::Create("resource/shaders/renderer2D_circle.glsl");
		#pragma endregion				 

		#pragma region Texture
		auto whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(uint32_t));
		s_Data.TextureSlots[0] = whiteTexture;
		#pragma endregion

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorageData::CameraData), 0);
	}

	void Renderer2D::ShutDown() 
	{
		delete[] s_Data.QuadVertexArrayBufferBase;
		delete[] s_Data.CircleVertexArrayBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{		
		/*s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		
		s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());*/
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorageData::CameraData));
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		/*s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", camera.GetProjection() * transform);
		s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", camera.GetProjection() * transform);*/
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * transform;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorageData::CameraData));
		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		/*s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", camera.GetProjection() * camera.GetViewMatrix());
		s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", camera.GetProjection() * camera.GetViewMatrix());*/
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * camera.GetViewMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorageData::CameraData));
		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		

		if (s_Data.CirlceIndexedCount)
		{
			uint32_t size = (uint8_t*)s_Data.CircleVertexArrayBufferPtr - (uint8_t*)s_Data.CircleVertexArrayBufferBase;
			s_Data.CircleVB->SetData(s_Data.CircleVertexArrayBufferBase, size);
			s_Data.CircleShader->Bind();
			RendererCommand::DrawIndexed(s_Data.CircleVA, s_Data.CirlceIndexedCount);
			//s_Data.CircleShader->unBind();
			s_Data.Stats.DrawCalls++;
		}

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
			//s_Data.QuadShader->unBind();
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityId)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, entityId);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color, entityId);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, int entityId, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, entityId, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, int entityId, float tilingFactor, const glm::vec4& tintColor)
	{		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });		
		DrawQuad(transform, texture, tilingFactor, tintColor, entityId);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, int entityId, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, entityId, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, int entityId, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subTexture, tilingFactor, tintColor, entityId);
	}
	
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
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
			s_Data.QuadVertexArrayBufferPtr->EntityId = entityId;
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityId)
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
			s_Data.QuadVertexArrayBufferPtr->EntityId = entityId;
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int entityId)
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
			s_Data.QuadVertexArrayBufferPtr->EntityId = entityId;
			s_Data.QuadVertexArrayBufferPtr++;
		}
		s_Data.QuadIndexedCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSpirite(const glm::mat4& transform, const SpiriteRendererComponent& spirite, int entityId)
	{
		if (spirite.Texture)
		{
			DrawQuad(transform, spirite.Texture, spirite.TilingFactor, spirite.Color, entityId);
		}
		else 
		{
			DrawQuad(transform, spirite.Color, entityId);
		}
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const CircleRendererComponent& circle, int entityId)
	{
		for (size_t i = 0; i < s_Data.VerticeCount; i++)
		{
			s_Data.CircleVertexArrayBufferPtr->WorldPosition = glm::vec3(transform * s_Data.QuadPosition[i]);
			s_Data.CircleVertexArrayBufferPtr->LocalPosition = glm::vec3(s_Data.QuadPosition[i] * 2.0f);
			s_Data.CircleVertexArrayBufferPtr->Color = circle.Color;
			s_Data.CircleVertexArrayBufferPtr->Thinness = circle.Thinness;
			s_Data.CircleVertexArrayBufferPtr->Fade = circle.Fade;
			s_Data.CircleVertexArrayBufferPtr->EntityId = entityId;
			s_Data.CircleVertexArrayBufferPtr++;
		}
		s_Data.CirlceIndexedCount += 6;
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

		s_Data.CirlceIndexedCount = 0;
		s_Data.CircleVertexArrayBufferPtr = s_Data.CircleVertexArrayBufferBase;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
}