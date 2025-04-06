#pragma once
#include "Texture.h"
#include <unordered_map>

namespace Engine {

	class Material
	{
	public:
		void AddTexture(ETextureType type, std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture(ETextureType type) const;
		void OnImGuiRender(const std::string& name = "Material");
		const std::unordered_map<ETextureType, std::shared_ptr<Texture2D>>& GetAllTextures() const { return _Textures; }

		bool _bHighlight = false;
		float _metalness = 0.0f;
		float _roughness = 0.0f;
		float _shininess = 32.0f;
	private:
		std::unordered_map<ETextureType, std::shared_ptr<Texture2D>> _Textures;
	};
}