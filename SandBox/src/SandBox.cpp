#include <Kenshin.h>
#include <Kenshin/ImGuiLayer/ImGuiLayer.h>

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		Kenshin::ImGuiLayer* layer = new Kenshin::ImGuiLayer();
		PushOverLay(layer);
	}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}