#pragma once

#include "Engine/Renderer/Texture.h"
namespace Engine {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, const std::string& type);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual void CreateCheckerboardTexture() override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual uint32_t GetWidth() const override { return _Width; }
		virtual uint32_t GetHeight() const override { return _Height; }

		virtual std::string& GetFile() override { return _Path; }
		virtual std::string& GetType() override { return _Type; }

		virtual uint32_t GetTextureID() override { return _TextureID; }
	private:
		std::string _Path;
		std::string _Type;
		uint32_t _TextureID;
		uint32_t _Width, _Height, _Nmrchannels;
	};
}