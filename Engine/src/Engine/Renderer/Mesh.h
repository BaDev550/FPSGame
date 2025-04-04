#pragma once
#include <vector>
#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/VertexArrayBuffer.h"
#include "Engine/Renderer/Texture.h"

namespace Engine
{
	class Shader;
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
			: _Vertices(vertices), _Indices(indices), _Textures(textures)
		{
			setupMesh();
		};
		~Mesh() {};
	
		void setupMesh();
		void Draw(std::shared_ptr<Shader>& shader);
	public:
		std::shared_ptr<VertexArrayBuffer> _Mesh;
		std::vector<Vertex> _Vertices;
		std::vector<unsigned int> _Indices;
		std::vector<Texture> _Textures;
	};
}
