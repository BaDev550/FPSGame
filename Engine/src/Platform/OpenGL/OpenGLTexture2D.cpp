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
		if (!data || path.empty()) {
			std::cout << "Failed to load image at path:" << path << " Using fallback texture.";
			CreateCheckerboardTexture();
			return;
		}
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

	OpenGLTexture2D::OpenGLTexture2D(uint32_t textureID, uint32_t width, uint32_t height)
		: _TextureID(textureID), _Width(width), _Height(height)
	{
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_TextureID);
	}

	void OpenGLTexture2D::CreateCheckerboardTexture()
	{
		const int checkerSize = 8;
		const int texSize = 128;
		_Width = _Height = texSize;
		_Nmrchannels = 3;

		std::vector<unsigned char> data(_Width * _Height * _Nmrchannels);

		for (int y = 0; y < _Height; ++y) {
			for (int x = 0; x < _Width; ++x) {
				int checkerX = x / checkerSize;
				int checkerY = y / checkerSize;
				bool isWhite = (checkerX + checkerY) % 2 == 0;

				int index = (y * _Width + x) * _Nmrchannels;
				if (isWhite) {
					data[index + 0] = 200;
					data[index + 1] = 200;
					data[index + 2] = 200;
				}
				else {
					data[index + 0] = 50;
					data[index + 1] = 50;
					data[index + 2] = 50;
				}
			}
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &_TextureID);
		glTextureStorage2D(_TextureID, 1, GL_RGB8, _Width, _Height);
		glTextureParameteri(_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_TextureID, 0, 0, 0, _Width, _Height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
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