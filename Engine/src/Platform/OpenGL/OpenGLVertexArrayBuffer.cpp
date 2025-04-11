#include "epch.h"
#include "OpenGLVertexArrayBuffer.h"
#include "glad/glad.h"

#define DEBUG_BUFFER 0
namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Vec2: return GL_FLOAT;
		case ShaderDataType::Vec3: return GL_FLOAT;
		case ShaderDataType::Vec4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		default:
			ENGINE_ASSERT(false, "Wrong ShaderDataType");
			return 0;
		}
		return 0;
	}
	
	OpenGLVertexArrayBuffer::OpenGLVertexArrayBuffer() {
		glGenVertexArrays(1, &_BufferID);
	}
	
	void OpenGLVertexArrayBuffer::Bind() const
	{
		glBindVertexArray(_BufferID);
	}
	
	void OpenGLVertexArrayBuffer::Unbind() const
	{
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArrayBuffer::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");
		glBindVertexArray(_BufferID);
		vertexBuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
#if (DEBUG_BUFFER)
			std::cout << "Binding attribute at index " << index
				<< " with component count " << element.GetComponentCount()
				<< " and type " << ShaderDataTypeToOpenGLBaseType(element.Type) << std::endl;
#endif // _DEBUG_BUFFER
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}
		_VertexBuffers.push_back(vertexBuffer);
	}
	
	void OpenGLVertexArrayBuffer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_BufferID);
		indexBuffer->Bind();
		_IndexBuffer = indexBuffer;
	}
}