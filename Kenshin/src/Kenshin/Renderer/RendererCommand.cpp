#include "kspch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Kenshin
{
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}