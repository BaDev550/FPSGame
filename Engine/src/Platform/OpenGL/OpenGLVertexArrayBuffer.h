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
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return _IndexBuffer; }
	
	private:
		std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
		std::shared_ptr<IndexBuffer> _IndexBuffer;
		uint32_t _BufferID;
	};
}