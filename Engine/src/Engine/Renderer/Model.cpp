#include "epch.h"
#include "Model.h"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices);
	
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
	
	void Model::Draw(std::shared_ptr<Shader>& shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->SetMat4("u_Model", model);
		for (auto & mesh : _Meshes)
			mesh.Draw(shader);
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
			glm::vec3 vector;
	
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position[0] = (float)vector.x;
			vertex.Position[1] = (float)vector.y;
			vertex.Position[2] = (float)vector.z;
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal[0] = (float)vector.x;
				vertex.Normal[1] = (float)vector.y;
				vertex.Normal[2] = (float)vector.z;
			}
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords[0] = (float)vec.x;
				vertex.TexCoords[1] = (float)vec.y;
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent[0] = (float)vector.x;
				vertex.Tangent[1] = (float)vector.y;
				vertex.Tangent[2] = (float)vector.z;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent[0] = (float)vector.x;
				vertex.Bitangent[1] = (float)vector.y;
				vertex.Bitangent[2] = (float)vector.z;
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
	
		// 1. diffuse maps
		std::vector<std::shared_ptr<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", _ModelDirectory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
		// 2. specular maps
		std::vector<std::shared_ptr<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", _ModelDirectory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
		// 3. normal maps
		std::vector<std::shared_ptr<Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", _ModelDirectory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
		// 4. height maps
		std::vector<std::shared_ptr<Texture2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", _ModelDirectory);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures);
	}
}
