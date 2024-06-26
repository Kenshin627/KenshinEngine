#include "kspch.h"
#include "Kenshin.h"
#include "ExampleLayer.h"

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		PushLayer(new Example("Example"));
	}
	~SandBox() {}
};

Kenshin::Application* Kenshin::CreateApplication()
{
	return new SandBox();
}