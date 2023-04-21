#pragma once
#include "Kenshin.h"

class SandBox2D : public Kenshin::Layer
{
public:
	SandBox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Kenshin::TimeStamp ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Kenshin::Event& e) override;
private:
	Kenshin::OrthoGraphicCameraController m_CameraController;
	Kenshin::Ref<Kenshin::Texture2D> m_checkboardTexture;
	Kenshin::Ref<Kenshin::Texture2D> m_BandTexture;
	Kenshin::Ref<Kenshin::Texture2D> m_SpiriteSheet;
	Kenshin::Ref<Kenshin::Texture2D> m_SpirteAnima;
	Kenshin::Ref<Kenshin::SubTexture2D> m_Tree;
	Kenshin::Ref<Kenshin::SubTexture2D> m_Bush;
	Kenshin::Ref<Kenshin::SubTexture2D> m_Pig;
	Kenshin::Ref<Kenshin::SubTexture2D> m_Cat;
};