#include "Kenshin.h"
#include "EditLayer.h"
#include "Kenshin/Core/EntityPoint.h"

namespace Kenshin
{
	class KenshinEditor : public Application
	{
	public:
		KenshinEditor(): Application(WindowProps("Kenshin-Editor"))
		{
			PushLayer(new EditLayer());
		}
		~KenshinEditor() {};
	};

	Application* createApplication(int argc, char** argv)
	{
		return new KenshinEditor();
	}
}