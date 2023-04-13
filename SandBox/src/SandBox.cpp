#include <Kenshin.h>
#include "imgui.h"
#include <glm.hpp>

class ExampleLayer :public Kenshin::Layer
{
public:
	ExampleLayer() :Layer("Example") {}
	void  OnUpdate() override
	{
		if (Kenshin::Input::IsKeyPressed(Kenshin::Key::Tab))
		{
			KS_INFO("TAB is pressed!");
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("TEST");
		ImGui::Text("test");
		ImGui::End();
	}
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