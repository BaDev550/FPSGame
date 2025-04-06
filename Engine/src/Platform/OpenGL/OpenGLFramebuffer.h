#pragma once
#include "Engine/Renderer/FrameBuffer.h"

namespace Engine
{
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t GetColorAttachmentFramebufferID() const override { return _ColorAttachment; };

	private:
		void Invalidate();
	private:
		uint32_t _FramebufferID = 0;
		uint32_t _ColorAttachment = 0;
		uint32_t _DepthStencilAttachment = 0;
		FrameBufferSpecification _Spec;
	};
}

