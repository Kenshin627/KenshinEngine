#include "Kenshin.h"

class SandBox :public Kenshin::Application
{
public:
	SandBox() {}
	~SandBox() {}
};

Kenshin::Application* Kenshin::CreateApplication()
{
	return new SandBox();
}