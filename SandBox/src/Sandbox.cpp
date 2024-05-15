#include "Kenshin.h"

class SandBox :public Kenshin::Application
{
public:
	SandBox() {}
	~SandBox() {}
};

Kenshin::Application* Kenshin::CreateApplication()
{
	KS_CORE_TRACE("from client");
	KS_CLIENT_ERROR("error from client");
	return new SandBox();
}