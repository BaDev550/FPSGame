#include "epch.h"
#include "OpenGLBuffers.h"
#include "glad/glad.h"

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex* vertices, uint32_t size)
	{
		glGenBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteVertexArrays(1, &_BufferID);
	}
	
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
	}
	
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	// Element Buffer
	
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: _Count(count)
	{
		glGenBuffers(1, &_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteVertexArrays(1, &_BufferID);
	}
	
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferID);
	}
	
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}