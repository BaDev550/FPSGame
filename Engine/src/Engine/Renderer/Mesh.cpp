#include "epch.h"
#include "Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	void Mesh::setupMesh()
	{
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(_Vertices.data(), _Vertices.size() * sizeof(Vertex)));
	
		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aTexCoord" },
			{ ShaderDataType::Vec3, "aNormal" },
			{ ShaderDataType::Vec3, "aTangent" },
			{ ShaderDataType::Vec3, "aBitangent" },
			{ ShaderDataType::Vec4, "aBoneIDs" },
			{ ShaderDataType::Vec4, "aWeights" }
		};
		vertexBuffer->SetLayout(layout);
	
		_Mesh.reset(VertexArrayBuffer::Create());
		_Mesh->AddVertexBuffer(vertexBuffer);
		
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(_Indices.data(), _Indices.size()));
	
		_Mesh->SetIndexBuffer(indexBuffer);
	}
	
	void Mesh::Draw(std::shared_ptr<Shader>& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (const auto & Texture : _Textures)
		{
			std::string number;
			std::string texType = Texture->GetType();
			if (texType == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (texType == "texture_specular")
				number = std::to_string(specularNr++);
			else if (texType == "texture_normal")
				number = std::to_string(normalNr++);
			else if (texType == "texture_height")
				number = std::to_string(heightNr++);

			Renderer::Submit(shader, Texture, _Mesh);
		}
	}
}
