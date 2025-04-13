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
#include <map>

namespace AssimpGLMHelper {
	inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from) {
		glm::mat4 to;
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
}

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
		void DrawShadow(std::shared_ptr<Shader>& ShadowShader, const glm::mat4& transform, glm::mat4 LightSpaceMatrix);
		std::vector<std::shared_ptr<Material>> GetAllMaterials();
		std::string& GetPath() { return _ModelPath; }
		std::vector<Mesh>& GetAllMeshes() { return _Meshes; }
		void SetMaterial(std::vector<std::shared_ptr<Material>>& materials) {
			size_t materialCount = materials.size();
			size_t meshCount = _Meshes.size();

			for (size_t i = 0; i < meshCount; ++i) {
				_Meshes[i]._Material = materials[i % materialCount];
			}
		}
		inline std::map<std::string, int> GetBoneMapping() { return _BoneMapping; }
	private:
		std::vector<Mesh> _Meshes;
		std::string _ModelDirectory;
		std::string _ModelPath;
		std::vector<std::shared_ptr<Texture2D>> _LoadedTextures;
		std::map<std::string, int> _BoneMapping;
		std::vector<glm::mat4> _BoneOffsets;
		int _BoneCounter = 0;

		std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
		void SetVertexBoneData(Vertex& vertex, int BoneID, float weight);
		void ExtractBoneWeight(std::vector<Vertex>& vertices, aiMesh* mesh);
	};
}
