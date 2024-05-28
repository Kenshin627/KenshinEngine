#include "ExampleLayer.h"
#include "imgui.h"
#include "Kenshin/Core.h"
#include "Kenshin/Input.h"
#include "Kenshin/Application.h"
#include <GLFW/include/GLFW/glfw3.h>

Example::Example(const std::string& name) 
	:Layer(name),
	m_CameraController(std::make_shared<Kenshin::OrthographicCameraController>(1920.0f / 1080.0f, false))
{
	//temp
	float vertices[4 * 5] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	const char* vertexSource = R"(
			#version 330 core
	
			layout (location = 0) in vec3 a_Pos;
			layout (location = 1) in vec2 a_TexCoord;
			
			uniform mat4 u_ViewProjection;			
			
			out vec2 v_TexCoord;
			void main() {
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
			}
		 )";

	const char* fragmentSource = R"(
			#version 330 core
			
			uniform sampler2D u_Texture;			
			out vec4 color;	
			in  vec2 v_TexCoord;

			void main(){
			 color = texture(u_Texture, v_TexCoord);
			};
		)";

	m_ShaderLibrary.Load("res/shaders/textureShader.glsl");
	m_Texture = Kenshin::Texture2D::Create("res/images/fishmans.png");
	m_Texture->Bind();
	Kenshin::FrameBufferSpecification fbspec;
	fbspec.Width = m_ViewportSize.x;
	fbspec.Height = m_ViewportSize.y;
	m_FrameBuffer = Kenshin::FrameBuffer::Create(fbspec);
	Kenshin::Ref<Kenshin::VertexBuffer> vbo;
	vbo.reset(Kenshin::VertexBuffer::Create(vertices, sizeof(float) * 4 * 5));
	Kenshin::Ref<Kenshin::IndexBuffer> ibo;
	ibo.reset(Kenshin::IndexBuffer::Create(indices, 6));

	Kenshin::BufferLayout layout =
	{
		{ "a_Position", Kenshin::ShaderDataType::Float3, false },
		{ "a_TexCoord", Kenshin::ShaderDataType::Float2, false }
	};
	vbo->SetLayout(layout);
	m_VAO.reset(Kenshin::VertexArray::Create());
	m_VAO->AddVertexBuffer(vbo);
	m_VAO->SetIndeBuffer(ibo);
}
Example::~Example() {}

void Example::OnUpdate(Kenshin::Timestep ts)
{
	m_FrameBuffer->Bind();
	m_CameraController->OnUpdate(ts);
	
	Kenshin::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Kenshin::RenderCommand::Clear();
	
	Kenshin::Renderer::BeginScene(m_CameraController->GetCamera());
	auto shader = m_ShaderLibrary.Get("textureShader");
	Kenshin::Renderer::Submit(m_VAO, shader, m_Texture);
	Kenshin::Renderer::EndScene();
	m_FrameBuffer->UnBind();
}

void Example::OnEvent(Kenshin::Event& e)
{
	m_CameraController->OnEvent(e);
}

void Example::OnImGuiRender() const
{
	bool show = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &show, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				Kenshin::Application::Get()->Close();
			}
			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (show)
		ImGui::ShowDemoWindow(&show);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Scene");
	auto size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)m_FrameBuffer->GetColorAttachmentID(), size, { 1, 1 }, { 0, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}