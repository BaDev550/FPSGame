#pragma once
#include <string>

namespace Engine
{
	class Texture
	{
	public:
		Texture(const std::string& path, const std::string& type);
		~Texture();
	
		void Bind(const int index) const;
		void Unbind() const;
		inline unsigned int GetWidth() const { return _Width; }
		inline unsigned int GetHeight() const { return _Height; }
		inline unsigned int GetTextureID() const { return _TextureID; }
		inline const std::string& GetFilePath() const { return _FilePath; }
		inline const std::string& GetType() const { return _Type; }
	private:
		unsigned int _TextureID;
		std::string _FilePath;
		std::string _Type;
		int _Width, _Height, _NumOfChannels;
	};
}

