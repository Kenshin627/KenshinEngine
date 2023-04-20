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
#include "Kenshin/Renderer/FrameBuffer.h"

namespace Kenshin
{

	class EditLayer : public Layer
	{
	public:
		EditLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStamp ts) override;
		virtual void OnImGuiRender(TimeStamp ts) override;
		virtual void OnEvent(Event& e) override;
	private:
		OrthoGraphicCameraController m_CameraController;
		glm::vec4 m_SquareColor;
		Ref<Texture2D> m_checkboardTexture;
		Ref<Texture2D> m_BandTexture;
		Ref<Texture2D> m_SpiriteSheet;
		Ref<Texture2D> m_SpirteAnima;
		Ref<SubTexture2D> m_Tree;
		Ref<SubTexture2D> m_Bush;
		Ref<SubTexture2D> m_Pig;
		Ref<SubTexture2D> m_Cat;
		Ref<FrameBuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}