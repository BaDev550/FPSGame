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
		if (!_Material)
			return;

		shader->Bind();
		shader->SetBool("u_Material.Highlight", _Material->_bHighlight);

		int textureSlot = 0;
		for (const auto& [type, texture] : _Material->GetAllTextures())
		{
			if (!texture || !texture->GetTextureID())
				continue;

			std::string uniformName;

			switch (type)
			{
			case ETextureType::Diffuse:
				uniformName = "u_Material.texture_diffuse";
				break;
			case ETextureType::Specular:
				uniformName = "u_Material.texture_specular";
				break;
			case ETextureType::Normal:
				uniformName = "u_Material.texture_normal";
				break;
			case ETextureType::AmbientOcclusion:
				uniformName = "u_Material.texture_ao";
				break;
			}

			texture->Bind(textureSlot);
			shader->SetInt(uniformName, textureSlot);
			shader->SetFloat("u_Material.shininess", _Material->_shininess);
			shader->SetFloat("u_Material.metalness", _Material->_metalness);
			shader->SetFloat("u_Material.roughness", _Material->_roughness);
			textureSlot++;
		}
		Renderer::Submit(shader, _Mesh);
	}
}
