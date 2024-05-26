#pragma once
#include "kspch.h"
//Renderer/////////////////////////////////////////////
#include "Kenshin/Renderer/Buffer.h"
#include "Kenshin/Renderer/OrthographicCamera.h"
#include "Kenshin/Renderer/RenderCommand.h"
#include "Kenshin/Renderer/Renderer.h"
#include "Kenshin/Renderer/Shader.h"
#include "Kenshin/Renderer/VertexArray.h"
#include "Kenshin/Layer.h"

#include "Kenshin/Core/Timestep.h"

class Example :public Kenshin::Layer
{
public:
	Example(const std::string& name);
	~Example();
	virtual void OnEvent(Kenshin::Event& e) override;
	virtual void OnImGuiRender() const override;
	virtual void OnUpdate(Kenshin::Timestep ts) override;
private:
	Kenshin::Ref<Kenshin::Shader> m_Shader;
	Kenshin::Ref<Kenshin::VertexArray> m_VAO;
	Kenshin::Ref<Kenshin::OrthographicCamera> m_Camera;
	float m_RotationSpeed = 5.0f;;
	float m_MoveSpeed = 5.0f;;
	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0f;
};