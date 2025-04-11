#include "epch.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"

namespace Engine {
	std::vector<Vertex> vertices;

	OpenGLFramebuffer::OpenGLFramebuffer(const FrameBufferSpecification& spec)
		: _Spec(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_FramebufferID);
		glDeleteTextures(1, &_ColorAttachment);
		glDeleteTextures(1, &_DepthAttachment);
		glDeleteRenderbuffers(1, &_DepthStencilAttachment);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _FramebufferID);
		//glViewport(0, 0, _Spec.Width, _Spec.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		_Spec.Width = width;
		_Spec.Height = height;
		Invalidate();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		vertices.clear();
		vertices.push_back({ { -1.0f,  1.0f, 0.0f }, {0.0f, 1.0f} });
		vertices.push_back({ { -1.0f, -1.0f, 0.0f }, {0.0f, 0.0f} });
		vertices.push_back({ {  1.0f, -1.0f, 0.0f }, {1.0f, 0.0f} });
		vertices.push_back({ { -1.0f,  1.0f, 0.0f }, {0.0f, 1.0f} });
		vertices.push_back({ {  1.0f, -1.0f, 0.0f }, {1.0f, 0.0f} });
		vertices.push_back({ {  1.0f,  1.0f, 0.0f }, {1.0f, 1.0f} });

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices.data(), vertices.size()));

		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aTexCoords" },
		};
		vertexBuffer->SetLayout(layout);

		_FrameBufferScreenBuffer.reset(VertexArrayBuffer::Create());
		_FrameBufferScreenBuffer->AddVertexBuffer(vertexBuffer);
		_FrameBufferScreenBuffer->SetVertexCount(vertices.size());

		if (_FramebufferID) {
			glDeleteFramebuffers(1, &_FramebufferID);
			glDeleteTextures(1, &_ColorAttachment);
			glDeleteRenderbuffers(1, &_DepthStencilAttachment);
		}

		if (_Spec.type == EFrameBufferType::ShadowMap)
		{
			glGenFramebuffers(1, &_FramebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, _FramebufferID);

			glGenTextures(1, &_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				_Spec.Width, _Spec.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Shadow Framebuffer is incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}
		else if (_Spec.type == EFrameBufferType::Default) {
			glGenFramebuffers(1, &_FramebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, _FramebufferID);

			glGenTextures(1, &_ColorAttachment);
			if (_Spec.UseMultisample) {
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _ColorAttachment);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA8, _Spec.Width, _Spec.Height, GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _ColorAttachment, 0);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, _ColorAttachment);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Spec.Width, _Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ColorAttachment, 0);
			}

			glGenRenderbuffers(1, &_DepthStencilAttachment);
			glBindRenderbuffer(GL_RENDERBUFFER, _DepthStencilAttachment);
			if (_Spec.UseMultisample)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, _Spec.Width, _Spec.Height);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _Spec.Width, _Spec.Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _DepthStencilAttachment);

			ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}