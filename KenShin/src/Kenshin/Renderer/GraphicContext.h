#pragma once

namespace Kenshin {
	class GraphicContext
	{
	public:
		virtual void Init() const = 0;
		virtual void SwapBuffers() const = 0;
	};
}