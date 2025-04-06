#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/VertexArrayBuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"

namespace Engine
{
	class Model
	{
	public:
		Model(const std::string& path) {
			std::cout << "Loading Model from " << path << std::endl;
			LoadModel(path);
		};
		void LoadModel(const std::string& path);
		void Draw(std::shared_ptr<Shader>& shader, std::shared_ptr<Shader>& ShadowShader, const glm::mat4& transform);
		std::vector<std::shared_ptr<Material>> GetAllMaterials();
		std::string& GetPath() { return _ModelPath; }
		std::vector<Mesh>& GetAllMeshes() { return _Meshes; }
	private:
		std::vector<Mesh> _Meshes;
		std::string _ModelDirectory;
		std::string _ModelPath;
		std::vector<std::shared_ptr<Texture2D>> _LoadedTextures;
	
		std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
	};
}
