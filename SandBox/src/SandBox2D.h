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
	Kenshin::Ref<Kenshin::VertexArray> m_QUADVAO;
	glm::mat4 m_QUADTransform;
	Kenshin::OrthoGraphicCameraController m_CameraController;
	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	Kenshin::ShaderLibrary m_ShaderLib;
};