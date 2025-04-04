#pragma once

#include "Engine/Renderer/Buffers.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(Vertex* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
	
		virtual void Bind() const override;
		virtual void Unbind() const override;
	
		virtual void SetLayout(const BufferLayout& layout) override { _Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return _Layout; }
	private:
		uint32_t _BufferID;
		BufferLayout _Layout;
	};
	
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
	
		virtual void Bind() const;
		virtual void Unbind() const;
		virtual uint32_t GetCount() const { return _Count; }
	private:
		uint32_t _BufferID;
		uint32_t _Count;
	};
}