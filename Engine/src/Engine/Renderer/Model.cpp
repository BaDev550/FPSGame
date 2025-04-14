#include "epch.h"
#include "Model.h"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	void Model::LoadModel(const std::string& path)
	{
		_ModelPath = path;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices
		);
	
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
	
		_ModelDirectory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}
	
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_Meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	
	}

	void Model::SetVertexBoneData(Vertex& vertex, int BoneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			if (vertex.Weights[i] == 0.0f) {
				vertex.BoneIDs[i] = BoneID;
				vertex.Weights[i] = weight;
				return;
			}
		}
	}

	void Model::ExtractBoneWeight(std::vector<Vertex>& vertices, aiMesh* mesh)
	{
		for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
			std::string boneName(mesh->mBones[i]->mName.C_Str());

			int boneIndex = 0;
			if (_BoneMapping.find(boneName) == _BoneMapping.end()) {
				boneIndex = _BoneCounter++;
				_BoneMapping[boneName] = boneIndex;
				_BoneOffsets.push_back(AssimpGLMHelper::ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix));
			}
			else {
				boneIndex = _BoneMapping[boneName];
			}

			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
				int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;
				SetVertexBoneData(vertices[vertexID], boneIndex, weight);
			}
		}
	}
	
	void Model::Draw(std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		glm::mat4 model = transform;
		shader->SetMat4("u_Model", model);

		for (auto& mesh : _Meshes) {
			mesh.Draw(shader);
		}
	}

	void Model::Draw(std::shared_ptr<Shader>& shader, const std::vector<glm::mat4>& instanceTransforms)
	{
		for (auto& mesh : _Meshes)
			mesh.Draw(shader, instanceTransforms);
	}

	void Model::DrawShadow(std::shared_ptr<Shader>& ShadowShader, const glm::mat4& transform, glm::mat4 LightSpaceMatrix)
	{
		glm::mat4 model = transform;
		ShadowShader->SetMat4("u_Model", model);

		for (auto& mesh : _Meshes) {
			mesh.DrawShadow(ShadowShader, LightSpaceMatrix);
		}
	}

	std::vector<std::shared_ptr<Material>> Model::GetAllMaterials()
	{
		std::vector<std::shared_ptr<Material>> materials;
		for (auto& mesh : _Meshes)
			materials.push_back(mesh.GetMaterial());

		return materials;
	}
	
	std::vector<std::shared_ptr<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory)
	{
		std::vector<std::shared_ptr<Texture2D>> textures;
		int textureCount = mat->GetTextureCount(type);
		for (unsigned int i = 0; i < textureCount; i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;
	
			bool skip = false;
			for (const auto & LoadedTexture : _LoadedTextures)
			{
				if (std::strcmp(LoadedTexture->GetFile().data(), filename.c_str()) == 0)
				{
					textures.push_back(LoadedTexture);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				std::cout << "Loading texture: " << filename << std::endl;
				std::shared_ptr<Texture2D> texture;
				texture = Texture2D::Create(filename, typeName);
				textures.push_back(texture);
				_LoadedTextures.push_back(texture);
			}
		}
		return textures;
	}
	
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture2D>> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.Position[0] = (float)mesh->mVertices[i].x;
			vertex.Position[1] = (float)mesh->mVertices[i].y;
			vertex.Position[2] = (float)mesh->mVertices[i].z;
			if (mesh->HasNormals())
			{
				vertex.Normal[0] = (float)mesh->mNormals[i].x;
				vertex.Normal[1] = (float)mesh->mNormals[i].y;
				vertex.Normal[2] = (float)mesh->mNormals[i].z;
			}
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords[0] = (float)mesh->mTextureCoords[0][i].x;
				vertex.TexCoords[1] = (float)mesh->mTextureCoords[0][i].y;
				// tangent
				vertex.Tangent[0] = (float)mesh->mTangents[i].x;
				vertex.Tangent[1] = (float)mesh->mTangents[i].y;
				vertex.Tangent[2] = (float)mesh->mTangents[i].z;
				// bitangent
				vertex.Bitangent[0] = (float)mesh->mBitangents[i].x;
				vertex.Bitangent[1] = (float)mesh->mBitangents[i].y;
				vertex.Bitangent[2] = (float)mesh->mBitangents[i].z;
			}
			else {
				vertex.TexCoords[0] = 0.0f;
				vertex.TexCoords[1] = 0.0f;
			}

			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::shared_ptr<Material> Mmaterial = std::make_shared<Material>();
		auto LoadAndSet = [&](aiTextureType type, ETextureType ourType, const std::string& name) {
			auto texs = loadMaterialTextures(material, type, name, _ModelDirectory);
			if (!texs.empty())
				Mmaterial->AddTexture(ourType, texs[0]);
			else
				Mmaterial->AddTexture(ourType, Texture2D::Create("", name));
		};
		LoadAndSet(aiTextureType_DIFFUSE, ETextureType::Diffuse, "texture_diffuse");
		LoadAndSet(aiTextureType_SPECULAR, ETextureType::Specular, "texture_specular");
		LoadAndSet(aiTextureType_HEIGHT, ETextureType::Normal, "texture_normal");
		LoadAndSet(aiTextureType_AMBIENT, ETextureType::AmbientOcclusion, "texture_ambient");

		return Mesh(vertices, indices, Mmaterial);
	}
}
