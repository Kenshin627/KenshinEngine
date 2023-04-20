#include "Kenshin.h"
#include "imgui.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "EditLayer.h"

namespace Kenshin
{
	class KenshinEditor :public Application
	{
	public:
		KenshinEditor()
		{
			PushOverLay(new EditLayer());
		}
		~KenshinEditor() {};
	};

	Application* createApplication(int argc, char** argv)
	{
		return new KenshinEditor();
	}
}