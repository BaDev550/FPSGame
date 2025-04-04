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
		for (unsigned int i = 0; i < _Meshes.size(); i++)
		{
			_Meshes[i].Draw(shader);
		}
	}
	
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory)
	{
		std::vector<Texture> textures;
		int textureCount = mat->GetTextureCount(type);
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
	
			bool skip = false;
			for (unsigned int j = 0; j < _LoadedTextures.size(); j++)
			{
				if (std::strcmp(_LoadedTextures[j].GetFilePath().data(), str.C_Str()) == 0)
				{
					textures.push_back(_LoadedTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				std::string filename = std::string(str.C_Str());
				filename = directory + '/' + filename;
	
				Texture texture(filename, typeName);
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
		std::vector<Texture> textures;
	
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
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", _ModelDirectory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", _ModelDirectory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", _ModelDirectory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", _ModelDirectory);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	
		return Mesh(vertices, indices, textures);
	}
}
