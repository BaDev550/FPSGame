#pragma once
#include <string>
#include "Engine/Core/Core.h"

namespace Engine
{
	enum class ETextureType
	{
		Diffuse,
		Specular,
		Normal,
		AmbientOcclusion
	};
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual void CreateCheckerboardTexture() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual std::string& GetFile() = 0;
		virtual std::string& GetType() = 0;

		virtual uint32_t GetTextureID() = 0;
	};

	class Texture2D : public Texture {
	public:
		static std::shared_ptr<Texture2D> Create(const std::string& path, const std::string& type);
	};
}

