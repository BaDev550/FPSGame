#include "epch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Engine {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const std::string& type)
		: _Path(path), _Type(type)
	{
		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ENGINE_ASSERT(data, "Failed to load image!");
		_Width = width;
		_Height = height;
		_Nmrchannels = channels;

		GLenum format{};
		if (_Nmrchannels == 1)
			format = GL_RED;
		if (_Nmrchannels == 3)
			format = GL_RGB;
		if (_Nmrchannels == 4)
			format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &_TextureID);
		glTextureStorage2D(_TextureID, 1, GL_RGB8, _Width, _Height);
		glTextureParameteri(_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(_TextureID, 0, 0, 0, _Width, _Height, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_TextureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, _TextureID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTextureUnit(0, 0);
	}

}