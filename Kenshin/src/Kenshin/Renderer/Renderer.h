#pragma once

namespace Kenshin
{
	enum RenderAPI
	{
		None, OpenGL
	};

	class Renderer
	{
	public:
		virtual ~Renderer() {}
		inline static RenderAPI GetAPI() { return s_RenderAPI; };
	private:
		static RenderAPI s_RenderAPI;
	};
}