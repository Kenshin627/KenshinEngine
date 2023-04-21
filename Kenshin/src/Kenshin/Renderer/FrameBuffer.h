#pragma once
#include "Kenshin/Core/Core.h"

namespace Kenshin
{

	struct FrameBufferSpecification
	{
		FrameBufferSpecification(uint32_t width = 0, uint32_t height = 0, uint32_t samples = 1, bool swapchain = false) : Width(width), Height(height), Samples(samples), SwapChainTarget(swapchain) {}
		uint32_t Width       = 0;
		uint32_t Height      = 0;
		uint32_t Samples     = 1;
		bool SwapChainTarget = false;
	};

	class KENSHIN_API FrameBuffer
	{
	public:
		virtual~FrameBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetColorAttachment() const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	private:

	};
}