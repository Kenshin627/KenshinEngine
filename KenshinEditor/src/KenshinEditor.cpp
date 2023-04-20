#include "Kenshin.h"
#include "imgui.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "EditLayer.h"

class KenshinEditor :public Kenshin::Application
{
public:
	KenshinEditor()
	{
		PushOverLay(new EditLayer());
	}
	~KenshinEditor() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new KenshinEditor();
}