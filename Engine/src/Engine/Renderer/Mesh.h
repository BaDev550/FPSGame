#pragma once
#include <vector>
#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/VertexArrayBuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class Shader;
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<Material>& material)
			: _Vertices(vertices), _Indices(indices), _Material(material)
		{
			setupMesh();
		};
		~Mesh() {};
		
		std::shared_ptr<Material> GetMaterial() const { return _Material; }
		void setupMesh();
		void setupInstancing();

		void Draw(std::shared_ptr<Shader>& shader, const std::vector<glm::mat4>& instanceMatrices = {});
		void DrawShadow(std::shared_ptr<Shader>& shader, glm::mat4 lightSpaceMatrix);

		void UpdateInstanceData(const std::vector<glm::mat4>& instanceMatrices);
	public:
		std::shared_ptr<VertexArrayBuffer> _Mesh;
		std::shared_ptr<VertexBuffer> _InstanceBuffer;
		std::vector<Vertex> _Vertices;
		std::vector<unsigned int> _Indices;
		std::shared_ptr<Material> _Material;
		std::vector<glm::mat4> _InstanceMatrices;
	};
}
