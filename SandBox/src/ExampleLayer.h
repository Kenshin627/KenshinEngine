#pragma once
#include "kspch.h"
#include "Kenshin/events/Event.h"
#include "Kenshin/Layer.h"

class Example :public Kenshin::Layer
{
public:
	Example(const std::string& name);
	~Example();
	virtual void OnEvent(Kenshin::Event& e) override;
};