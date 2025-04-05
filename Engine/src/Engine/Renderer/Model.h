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
		void Draw(std::shared_ptr<Shader>& shader, const glm::mat4& transform);
	private:
		std::vector<Mesh> _Meshes;
		std::vector<std::shared_ptr<Texture2D>> _LoadedTextures;
		std::string _ModelDirectory;
	
		std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
	};
}
