#pragma once
#include "Kenshin/Core/Core.h"
#include <vector>

namespace Kenshin
{

	enum class FrameBufferTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	enum class FrameBufferTextureFilter
	{
		LINEAR,
		NEAREAST
	};

	enum class FrameBufferTextureWrap
	{
		CLAMPTOEDGE,
		CLAMPTOBORDER,
		MIRROREDREPEAT,
		REPEAT,
		MIRRORCLAMPTOEDGE
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format) :TextureFormat(format) {}
		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> list) : attachments(list) {}
		std::vector<FrameBufferTextureSpecification> attachments;
	};


	struct FrameBufferSpecification
	{
		FrameBufferSpecification(uint32_t width = 0, uint32_t height = 0, uint32_t samples = 1, bool swapchain = false) : Width(width), Height(height), Samples(samples), SwapChainTarget(swapchain) {}
		uint32_t Width       = 0;
		uint32_t Height      = 0;
		FrameBufferAttachmentSpecification attachments;
		uint32_t Samples     = 1;
		bool SwapChainTarget = false;
	};

	class  FrameBuffer
	{
	public:
		virtual~FrameBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void DeleteFrameBuffer() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t attachmentIndex = 0) const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	private:

	};
}