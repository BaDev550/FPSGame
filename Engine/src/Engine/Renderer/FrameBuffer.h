#pragma once

namespace Engine {
	struct FrameBufferSpecification
	{
		uint32_t Width = 1280, Height = 720;
		bool UseMultisample = false;
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetColorAttachmentFramebufferID() const = 0;

		static FrameBuffer* Create(const FrameBufferSpecification& spec);
	};
}