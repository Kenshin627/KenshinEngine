#include <Kenshin.h>

class ExampleLayer :public Kenshin::Layer
{
public:
	ExampleLayer() :Layer("Example") {}
	void  OnUpdate() override
	{
		//KS_INFO("ExampleLayer::Update");
		if (Kenshin::Input::IsKeyPressed(Kenshin::Key::Tab))
		{
			KS_INFO("TAB is pressed!");
		}
	}
};

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		Kenshin::ImGuiLayer* layer = new Kenshin::ImGuiLayer();
		PushOverLay(layer);
		PushOverLay(new ExampleLayer());
	}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}