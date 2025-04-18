#pragma once
#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/VertexArrayBuffer.h"

namespace Engine {
	enum class EFrameBufferType
	{
		Default,
		ShadowMap
	};
	struct FrameBufferSpecification
	{
		uint32_t Width = 1280, Height = 720;
		bool UseMultisample = false;
		EFrameBufferType type = EFrameBufferType::Default;
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetColorAttachmentFramebufferID() const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;
		virtual std::shared_ptr<VertexArrayBuffer> GetScreenBuffer() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static FrameBuffer* Create(const FrameBufferSpecification& spec);
	};
}