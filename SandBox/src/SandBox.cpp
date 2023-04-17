#include "Kenshin.h"
#include "imgui.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class ExampleLayer :public Kenshin::Layer
{
public:
	ExampleLayer() : m_CameraController(1280.0f / 720.0f, true),Layer("Example")
	{
		#pragma region TRIANGLE
		m_TRIANGLEVAO.reset(Kenshin::VertexArray::CreateVertexArray());
		float vertices[3 * 7] = {
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
			 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
			 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0
		};
		unsigned indices[3] = { 0, 1, 2 };
		Kenshin::Ref<Kenshin::VertexBuffer> triangleVBO = Kenshin::Ref<Kenshin::VertexBuffer>(Kenshin::VertexBuffer::CreateBuffer(vertices, sizeof(vertices)));
		Kenshin::VertexBufferLayout layout =
		{
			{ "aPosition", Kenshin::ShaderDataType::Float3 },
			{ "aColor",    Kenshin::ShaderDataType::Float4 }
		};
		triangleVBO->SetLayout(layout);
		m_TRIANGLEVAO->AddVertexBuffer(triangleVBO);
		Kenshin::Ref<Kenshin::IndexBuffer> triangleEBO = Kenshin::Ref<Kenshin::IndexBuffer>(Kenshin::IndexBuffer::CreateBuffer(indices, 3));
		m_TRIANGLEVAO->SetIndexBuffer(triangleEBO);
		#pragma endregion

		#pragma region QUAD
		m_QUADVAO.reset(Kenshin::VertexArray::CreateVertexArray());
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
		m_QUADVAO->AddVertexBuffer(quadVBO);
		Kenshin::Ref<Kenshin::IndexBuffer> quadEBO = Kenshin::Ref<Kenshin::IndexBuffer>(Kenshin::IndexBuffer::CreateBuffer(quadIndices, 6));
		m_QUADVAO->SetIndexBuffer(quadEBO);
		#pragma endregion

		auto flatColorShader = m_ShaderLib.Load("../Kenshin/resource/shaders/flatColor.glsl");
		auto textureShader = m_ShaderLib.Load("../Kenshin/resource/shaders/texture.glsl");

		m_Texture = Kenshin::Ref<Kenshin::Texture2D>(Kenshin::Texture2D::Create("resurce/textures/Checkerboard.png"));
		m_LogTexture = Kenshin::Ref<Kenshin::Texture2D>(Kenshin::Texture2D::Create("resurce/textures/ChernoLogo.png"));
		
		textureShader->Bind();
		textureShader->SetInt("sampler", 0);
	}
	void  OnUpdate(Kenshin::TimeStamp ts) override
	{
		m_CameraController.OnUpdate(ts);
		Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
		Kenshin::RendererCommand::Clear();
		Kenshin::Renderer::BeginScene(m_CameraController.GetCamera());
		auto flatColorShader = m_ShaderLib.Get("flatColor");
		auto textureShader = m_ShaderLib.Get("texture");
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
		flatColorShader->Bind();
		flatColorShader->SetVec3("u_Color", m_SquareColor);
		for (unsigned x = 0; x < 20; x++)
		{
			for (unsigned y = 0; y < 20; y++) 
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0), pos) * scaleMatrix;
				Kenshin::Renderer::Submit(m_QUADVAO, flatColorShader, transform);
			}
		}
		
		m_Texture->Bind();
		Kenshin::Renderer::Submit(m_QUADVAO, textureShader, glm::mat4(1.0));
		m_LogTexture->Bind();
		Kenshin::Renderer::Submit(m_QUADVAO, textureShader, glm::mat4(1.0));
		Kenshin::Renderer::EndScene();
	}

	void OnEvent(Kenshin::Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("squareColor");
		ImGui::ColorEdit3("square", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Kenshin::Ref<Kenshin::VertexArray> m_TRIANGLEVAO;
	Kenshin::Ref<Kenshin::VertexArray> m_QUADVAO;
	glm::mat4 m_QUADTransform;
	Kenshin::OrthoGraphicCameraController m_CameraController;
	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	Kenshin::Ref<Kenshin::Texture2D> m_Texture;
	Kenshin::Ref<Kenshin::Texture2D> m_LogTexture;
	Kenshin::ShaderLibrary m_ShaderLib;
};

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		PushOverLay(new ExampleLayer());
	}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}