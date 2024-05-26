#include "ExampleLayer.h"
#include "imgui.h"
#include "Kenshin/Core.h"
#include "Kenshin/Input.h"
#include <GLFW/include/GLFW/glfw3.h>

Example::Example(const std::string& name) 
	:Layer(name),
	m_Camera(std::make_shared<Kenshin::OrthographicCamera>(-2.0f, 2.0f, -0.56f * 2.0f, 0.56f * 2.0f, -1.0f, 1.0f))
{
	//temp
	float vertices[4 * 7] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};

	uint32_t indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	const char* vertexSource = R"(
			#version 330 core
	
			layout (location = 0) in vec3 a_Pos;
			layout (location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;			

			out vec4 v_Color;
			void main() {
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
			}
		 )";

	const char* fragmentSource = R"(
			#version 330 core
			
			out vec4 color;	
			in  vec4 v_Color;
			void main(){
			 color = v_Color;
			};
		)";
	m_Shader = Kenshin::Shader::Create(vertexSource, fragmentSource);

	Kenshin::Ref<Kenshin::VertexBuffer> vbo;
	vbo.reset(Kenshin::VertexBuffer::Create(vertices, sizeof(float) * 4 * 7));
	Kenshin::Ref<Kenshin::IndexBuffer> ibo;
	ibo.reset(Kenshin::IndexBuffer::Create(indices, 6));

	Kenshin::BufferLayout layout =
	{
		{ "a_Position", Kenshin::ShaderDataType::Float3, false },
		{ "a_Color", Kenshin::ShaderDataType::Float4, false }
	};
	vbo->SetLayout(layout);
	m_VAO.reset(Kenshin::VertexArray::Create());
	m_VAO->AddVertexBuffer(vbo);
	m_VAO->SetIndeBuffer(ibo);
}
Example::~Example() {}

void Example::OnUpdate(Kenshin::Timestep ts)
{
	if (Kenshin::Input::IsKeyPressed(GLFW_KEY_LEFT))
	{
		m_Position.x += m_MoveSpeed * ts;
	}
	else if (Kenshin::Input::IsKeyPressed(GLFW_KEY_RIGHT))
	{
		m_Position.x -= m_MoveSpeed * ts;
	}

	if (Kenshin::Input::IsKeyPressed(GLFW_KEY_UP))
	{
		m_Position.y -= m_MoveSpeed * ts;
	}
	else if (Kenshin::Input::IsKeyPressed(GLFW_KEY_DOWN))
	{
		m_Position.y += m_MoveSpeed * ts;
	}

	if (Kenshin::Input::IsKeyPressed(GLFW_KEY_A))
	{
		m_Rotation -= m_RotationSpeed * ts;
	}
	else if (Kenshin::Input::IsKeyPressed(GLFW_KEY_D))
	{
		m_Rotation += m_RotationSpeed * ts;
	}

	m_Camera->SetPosition(m_Position);
	m_Camera->SetRotation(m_Rotation);


	Kenshin::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Kenshin::RenderCommand::Clear();
	
	Kenshin::Renderer::BeginScene(m_Camera);
	Kenshin::Renderer::Submit(m_VAO, m_Shader);
	Kenshin::Renderer::EndScene();
}

void Example::OnEvent(Kenshin::Event& e)
{
}

void Example::OnImGuiRender() const
{

}