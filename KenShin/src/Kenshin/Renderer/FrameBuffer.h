#pragma once
#include "Kenshin/Core.h"
#include <cstdint>

namespace Kenshin {
	struct FrameBufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;
		bool SwapchainTarget = false;
	};

	class KS_API FrameBuffer
	{
	public:
		FrameBuffer() {};
		virtual ~FrameBuffer() {};
		virtual void Invalicate() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual uint32_t GetColorAttachmentID() const = 0;
		static Ref<FrameBuffer> Create(FrameBufferSpecification& spec);
	};
}