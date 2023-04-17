#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Core/Layer.h"
#include "Kenshin/Core/TimeStamp.h"
#include "Kenshin/Events/Event.h"
#include "Kenshin/Renderer/VertexArray.h"
#include "Kenshin/Renderer/Texture.h"
#include "Kenshin/Renderer/OrthographicCameraController.h"
#include "Kenshin/Renderer/Shader.h"
#include "Kenshin/Renderer/Renderer.h"
#include "Kenshin/Renderer/Renderer2D.h"

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
	glm::vec4 m_SquareColor;
	Kenshin::Ref<Kenshin::Texture2D> m_checkboardTexture;
};