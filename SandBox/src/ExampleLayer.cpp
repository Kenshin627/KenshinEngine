#include "ExampleLayer.h"
#include "Kenshin/Log.h"
#include "imgui.h"

Example::Example(const std::string& name) :Layer(name) {}
Example::~Example() {}

void Example::OnEvent(Kenshin::Event& e)
{

}

void Example::OnImGuiRender() const
{
	ImGui::Text("hello sandBox");
}