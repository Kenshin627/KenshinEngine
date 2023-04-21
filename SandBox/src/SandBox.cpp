#include "Kenshin.h"
#include "SandBox2D.h"
#include "Kenshin/Core/EntityPoint.h"

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		PushOverLay(new SandBox2D());
	}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}