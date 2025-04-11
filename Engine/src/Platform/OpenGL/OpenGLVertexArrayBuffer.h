#pragma once
#include "Engine/Renderer/VertexArrayBuffer.h"

namespace Engine
{
	class OpenGLVertexArrayBuffer : public VertexArrayBuffer
	{
	public:
		OpenGLVertexArrayBuffer();
		virtual ~OpenGLVertexArrayBuffer() {}
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual void SetVertexCount(uint32_t count) override { _VertexCount = count; };
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return _IndexBuffer; }
		virtual uint32_t GetVertexCount() const override { return _VertexCount; };

	private:
		std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
		std::shared_ptr<IndexBuffer> _IndexBuffer;
		uint32_t _VertexCount;
		uint32_t _BufferID;
	};
}