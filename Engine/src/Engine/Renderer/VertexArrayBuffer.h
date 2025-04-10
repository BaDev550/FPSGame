#pragma once

#include "Engine/Renderer/Buffers.h"

namespace Engine
{
	class VertexArrayBuffer
	{
	public:
		virtual ~VertexArrayBuffer() {};
	
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual void SetVertexCount(uint32_t count) = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
		virtual uint32_t GetVertexCount() const = 0;

		static VertexArrayBuffer* Create();
	};
}