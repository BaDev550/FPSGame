#include "epch.h"
#include "Texture.h"
#include "Engine/Core/Core.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace Engine
{
	Texture::Texture(const std::string& path, const std::string& type)
	{
		glGenTextures(1, &_TextureID);
		_FilePath = path;
		_Type = type;
	
		unsigned char* data = stbi_load(path.c_str(), &_Width, &_Height, &_NumOfChannels, 0);
		if (data) {
			GLenum format{};
			if (_NumOfChannels == 1)
				format = GL_RED;
			else if (_NumOfChannels == 3)
				format = GL_RGB;
			else if (_NumOfChannels == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, _TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, _Width, _Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture: " << path << std::endl;
			stbi_image_free(data);
		}
	}
	
	Texture::~Texture()
	{
		glDeleteTextures(1, &_TextureID);
		_TextureID = 0;
		_Width = 0;
		_Height = 0;
		_NumOfChannels = 0;
		_FilePath.clear();
		_Type.clear();
	}
	
	void Texture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _TextureID);
	}
	
	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
