#include "Kenshin.h"
#include "imgui.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class ExampleLayer :public Kenshin::Layer
{
public:
	ExampleLayer() :Layer("Example") 
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

		m_Shader = Kenshin::Scope<Kenshin::Shader>(Kenshin::Shader::Create("../Kenshin/resource/shaders/flatColor.glsl"));
		m_TextureShader = Kenshin::Scope<Kenshin::Shader>(Kenshin::Shader::Create("../Kenshin/resource/shaders/texture.glsl"));
		m_Texture = Kenshin::Ref<Kenshin::Texture2D>(Kenshin::Texture2D::Create("resurce/textures/Checkerboard.png"));
		m_LogTexture = Kenshin::Ref<Kenshin::Texture2D>(Kenshin::Texture2D::Create("resurce/textures/ChernoLogo.png"));
		
		m_TextureShader->Bind();
		m_TextureShader->SetInt("sampler", 0);
		//temp: 16/9
		m_Camera = Kenshin::CreateRef<Kenshin::OrthographicCamera>(-2.0f, 2.0f, -9.0f / 8.0f, 9.0f / 8.0f, 2.0f);
	}
	void  OnUpdate(Kenshin::TimeStamp ts) override
	{
		Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
		Kenshin::RendererCommand::Clear();
		UpdateCamera(ts);
		Kenshin::Renderer::BeginScene(m_Camera);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
		m_Shader->Bind();
		m_Shader->SetVec3("u_Color", m_SquareColor);
		for (unsigned x = 0; x < 20; x++)
		{
			for (unsigned y = 0; y < 20; y++) 
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0), pos) * scaleMatrix;
				Kenshin::Renderer::Submit(m_QUADVAO, m_Shader, transform);
			}
		}
		
		m_Texture->Bind();
		Kenshin::Renderer::Submit(m_QUADVAO, m_TextureShader, glm::mat4(1.0));
		m_LogTexture->Bind();
		Kenshin::Renderer::Submit(m_QUADVAO, m_TextureShader, glm::mat4(1.0));
		Kenshin::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("squareColor");
		ImGui::ColorEdit3("square", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void UpdateCamera(Kenshin::TimeStamp ts)
	{
		KS_INFO("seconds:{0}s ({1}ms)", ts.GetSeconds(), ts.GetmileSeconds());
		float deltaTime = ts;
		if (Kenshin::Input::IsKeyPressed(Kenshin::Key::W))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera->GetSpeed() * deltaTime);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::S))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera->GetSpeed() * deltaTime);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::A))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * m_Camera->GetSpeed() * deltaTime);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::D))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::vec3(1.0f, 0.0f, 0.0f) * m_Camera->GetSpeed() * deltaTime);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::Q))
		{
			m_Camera->SetRotation(m_Camera->GetRotation() + 1.0f * deltaTime);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::E))
		{
			m_Camera->SetRotation(m_Camera->GetRotation() - 1.0f * deltaTime);
		}
	}
private:
	Kenshin::Ref<Kenshin::VertexArray> m_TRIANGLEVAO;
	Kenshin::Ref<Kenshin::VertexArray> m_QUADVAO;
	glm::mat4 m_QUADTransform;
	Kenshin::Ref<Kenshin::Shader> m_Shader;
	Kenshin::Ref<Kenshin::Shader> m_TextureShader;
	Kenshin::Ref<Kenshin::OrthographicCamera> m_Camera;
	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	Kenshin::Ref<Kenshin::Texture2D> m_Texture;
	Kenshin::Ref<Kenshin::Texture2D> m_LogTexture;
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