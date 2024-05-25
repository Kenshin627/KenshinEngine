#include "ExampleLayer.h"
#include "Kenshin/Log.h"
#include "imgui.h"

Example::Example(const std::string& name) :Layer(name) {}
Example::~Example() {}

void Example::OnEvent(Kenshin::Event& e)
{
	KS_CLIENT_DEBUG("from Example: {0}",e.ToString());
}

void Example::OnImGuiRender() const
{
	ImGui::Text("hello sandBox");
}