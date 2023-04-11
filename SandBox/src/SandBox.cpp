#include <Kenshin.h>

class SandBox :public Kenshin::Application
{
public:
	SandBox() {}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}