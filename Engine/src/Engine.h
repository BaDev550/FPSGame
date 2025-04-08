#pragma once

#include "Engine/Core/Application/Application.h"
#include "Engine/Core/Application/EntiryPoint.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h"
#include "Engine/Input/MouseButtonCodes.h"

#include "Engine/Window/Layer.h"
#include "Engine/Window/ImGui/ImGuiLayer.h"

#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/GraphicsContext.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArrayBuffer.h"
#include "Engine/Renderer/Model.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ECS/Components.h"
#include "Engine/Scene/ECS/Systems.h"

#include "yaml-cpp/yaml.h"

typedef Engine::Application EngineApp;
typedef Engine::Layer EngineLayer;
typedef Engine::Event EngineEvent;
typedef Engine::Window EngineWindow;
typedef Engine::Input EngineInput;
typedef Engine::Shader EngineShader;
typedef Engine::VertexArrayBuffer EngineVertexArrayBuffer;
typedef Engine::RenderCommand EngineRenderCommand;
typedef Engine::GraphicsContext EngineGraphicsContext;
typedef Engine::Renderer EngineRenderer;
typedef Engine::Model EngineModel;
typedef Engine::BufferLayout EngineBufferLayout;
typedef Engine::BufferElement EngineBufferElement;
typedef Engine::Vertex EngineVertex;
typedef Engine::ShaderDataType EngineShaderDataType;
typedef Engine::Scene EngineScene;
typedef Engine::RenderSystem EngineRenderSystem;

typedef Engine::MeshComponent EngineStaticMeshComponent;
typedef Engine::NameComponent EngineNameComponent;
typedef Engine::TransformComponent EngineTransformComponent;
typedef Engine::ETextureType EngineTextureType;
typedef Engine::Material EngineMaterial;
typedef entt::entity Entity;

#define EngineLoadStaticMesh(path) std::unique_ptr<EngineModel>(new EngineModel(path))
#define EngineCreateShader(vertexPath, fragmentPath) std::shared_ptr<EngineShader>(new EngineShader(vertexPath, fragmentPath))

#define EngineKeyPressed(key) EngineInput::IsKeyPressed(key)
#define EngineLoadScene(path, scene) VortexEngine::LoadScene(path, scene);
#define EngineSaveScene(path, scene) VortexEngine::SaveScene(path, scene);

#define EngineClear() EngineRenderCommand::Clear()
#define EngineSetClearColor(r, g, b, a) EngineRenderCommand::SetClearColor({r, g, b, a})
#define EngineSetPollyMode(face, mode) EngineRenderCommand::SetPolyMode(face, mode);

#define RendererBeginScene(camera) EngineRenderer::BeginScene(camera)
#define RenderShadow(shader, vertexArray, lightSpaceMatrix) EngineRenderer::RenderShadowMap(shader, vertexArray, lightSpaceMatrix)
#define RendererEndScene() EngineRenderer::EndScene()
#define RendererSubmit(shader, vertexArray) EngineRenderer::Submit(shader, vertexArray)

namespace YAML {
	template<>
	struct convert<EngineTextureType> {
		static Node encode(const EngineTextureType& type) {
			Node node;
			switch (type) {
			case EngineTextureType::Diffuse:
				node = "texture_diffuse";
				break;
			case EngineTextureType::Normal:
				node = "texture_normal";
				break;
			case EngineTextureType::Specular:
				node = "texture_specular";
				break;
			case EngineTextureType::AmbientOcclusion:
				node = "texture_ambientOcclusion";
				break;
			default:
				node = "texture_unknown";
				break;
			}
			return node;
		}

		static bool decode(const Node& node, EngineTextureType& type) {
			if (!node.IsScalar()) {
				return false;
			}

			std::string val = node.as<std::string>();
			if (val == "texture_diffuse") {
				type = EngineTextureType::Diffuse;
			}
			else if (val == "texture_normal") {
				type = EngineTextureType::Normal;
			}
			else if (val == "texture_specular") {
				type = EngineTextureType::Specular;
			}
			else if (val == "texture_ambientOcclusion") {
				type = EngineTextureType::AmbientOcclusion;
			}
			else {
				type = EngineTextureType::Diffuse;
				std::cerr << "Warning: Unknown texture type '" << val << "', defaulting to 'texture_diffuse'.\n";
				return true;
			}
			return true;
		}
	};

	template<>
	struct convert<std::shared_ptr<Engine::Texture2D>> {
		static Node encode(const std::shared_ptr<Engine::Texture2D>& texture) {
			Node node;
			if (!texture)
				return node;

			node["File"] = texture->GetFile();
			node["Type"] = texture->GetType();
			return node;
		}

		static bool decode(const Node& node, std::shared_ptr<Engine::Texture2D>& texture) {
			if (!node["File"] || !node["Type"])
				return false;

			std::string file = node["File"].as<std::string>();
			std::string type = node["Type"].as<std::string>();
			texture = Engine::Texture2D::Create(file, type);
			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<EngineMaterial> {
		static Node encode(const EngineMaterial& mat) {
			Node node;

			node["Highlight"] = mat._bHighlight;
			node["Metalness"] = mat._metalness;
			node["Roughness"] = mat._roughness;
			node["Shininess"] = mat._shininess;

			Node textures;
			for (const auto& [type, texture] : mat.GetAllTextures()) {
				if (texture)
					textures[static_cast<int>(type)] = texture->GetFile();
			}
			node["Textures"] = textures;

			return node;
		}

		static bool decode(const Node& node, EngineMaterial& mat) {
			if (node["Highlight"])
				mat._bHighlight = node["Highlight"].as<bool>();
			if (node["Metalness"])
				mat._metalness = node["Metalness"].as<float>();
			if (node["Roughness"])
				mat._roughness = node["Roughness"].as<float>();
			if (node["Shininess"])
				mat._shininess = node["Shininess"].as<float>();

			if (node["Textures"]) {
				for (auto textureNode : node["Textures"]) {
					EngineTextureType type = textureNode.first.as<EngineTextureType>();
					std::shared_ptr<Engine::Texture2D> texture = textureNode.second.as<std::shared_ptr<Engine::Texture2D>>();
					mat.AddTexture(type, texture);
				}
			}

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const EngineTextureType& type) {
		switch (type) {
		case EngineTextureType::Diffuse:
			out << "texture_diffuse";
			break;
		case EngineTextureType::Normal:
			out << "texture_normal";
			break;
		case EngineTextureType::Specular:
			out << "texture_specular";
			break;
		case EngineTextureType::AmbientOcclusion:
			out << "texture_ambientOcclusion";
			break;
		default:
			out << "Unknown";
			break;
		}
		return out;
	}

	inline Emitter& operator<<(Emitter& out, const std::shared_ptr<Engine::Texture2D>& tex) {
		if (!tex) {
			out << Null;
			return out;
		}

		out << BeginMap;
		out << Key << "File" << Value << tex->GetFile();
		out << Key << "Type" << Value << tex->GetType();
		out << EndMap;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const EngineMaterial& mat) {
		out << YAML::BeginMap;
		out << YAML::Key << "Highlight" << YAML::Value << mat._bHighlight;
		out << YAML::Key << "Metalness" << YAML::Value << mat._metalness;
		out << YAML::Key << "Roughness" << YAML::Value << mat._roughness;
		out << YAML::Key << "Shininess" << YAML::Value << mat._shininess;

		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginMap;
		for (const auto& [type, tex] : mat.GetAllTextures()) {
			if (tex)
				out << YAML::Key << type << YAML::Value << tex;
		}
		out << YAML::EndMap;
		out << YAML::EndMap;
		return out;
	}

	inline Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
}

class VortexEngine {
public:
	static bool LoadScene(const std::string& path, EngineScene& scene) {
		auto& registry = scene.GetRegistry();
		auto view = registry.view<EngineNameComponent, EngineTransformComponent, EngineStaticMeshComponent>();
		for (auto entity : view) {
			if (registry.any_of<EngineNameComponent>(entity) || registry.any_of<EngineTransformComponent>(entity) || registry.any_of<EngineStaticMeshComponent>(entity)) {
				registry.destroy(entity);
			}
		}

		YAML::Node data;
		try {
			data = YAML::LoadFile(path);
		}
		catch (const YAML::Exception& e) {
			std::cout << "Failed to load scene file: " << e.what() << std::endl;
			return false;
		}

		auto entities = data["Entities"];
		if (!entities) {
			std::cout << "No entities found in scene file." << std::endl;
			return false;
		}

		scene.SetPath(path);
		for (auto entityNode : entities) {
			entt::entity entity = scene.GetRegistry().create();

			auto tagNode = entityNode["NameComponent"];
			if (tagNode) {
				auto& tag = scene.GetRegistry().emplace<EngineNameComponent>(entity);
				tag.name = tagNode["Name"].as<std::string>();
			}

			auto transformNode = entityNode["TransformComponent"];
			if (transformNode) {
				auto& tc = scene.GetRegistry().emplace<EngineTransformComponent>(entity);
				tc.Position = transformNode["Position"].as<glm::vec3>();
				tc.Rotation = transformNode["Rotation"].as<glm::vec3>();
				tc.Scale = transformNode["Scale"].as<glm::vec3>();
			}

			auto modelNode = entityNode["MeshComponent"];
			if (modelNode) {
				std::string path = modelNode["ModelPath"].as<std::string>();
				std::vector<std::shared_ptr<EngineMaterial>> materials;

				if (modelNode["ModelMaterials"].IsSequence()) {
					std::cout << "Found " << modelNode["ModelMaterials"].size() << " materials." << std::endl;
					for (auto matNode : modelNode["ModelMaterials"]) {
						EngineMaterial mat = matNode.as<EngineMaterial>();
						materials.push_back(std::make_shared<EngineMaterial>(mat));
					}
				}
				else {
					std::cout << "No ModelMaterials found or not a sequence!" << std::endl;
				}

				std::cout << "Total materials loaded: " << materials.size() << std::endl;

				auto& tc = scene.GetRegistry().emplace<EngineStaticMeshComponent>(entity, path);
				tc.SetMaterials(materials);
			}
		}
		return true;
	}
	static void SaveScene(const std::string& path, EngineScene& scene) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		//out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = scene.GetRegistry().view<EngineNameComponent, EngineTransformComponent, EngineStaticMeshComponent>();

		for (auto entity : view) {
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << (uint32_t)entity;

			if (scene.GetRegistry().any_of<EngineNameComponent>(entity)) {
				auto& tag = scene.GetRegistry().get<EngineNameComponent>(entity);
				out << YAML::Key << "NameComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Name" << YAML::Value << tag.name;
				out << YAML::EndMap;
			}

			if (scene.GetRegistry().any_of<EngineTransformComponent>(entity)) {
				auto& tc = scene.GetRegistry().get<EngineTransformComponent>(entity);
				out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Position" << YAML::Value << tc.Position;
				out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
				out << YAML::EndMap;
			}

			if (scene.GetRegistry().any_of<EngineStaticMeshComponent>(entity)) {
				auto& tc = scene.GetRegistry().get<EngineStaticMeshComponent>(entity);
				std::vector<EngineMaterial> materials;
				for (auto& material : tc.GetMaterials())
					materials.push_back(*material);

				out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "ModelPath" << YAML::Value << tc._Model->GetPath();
				out << YAML::Key << "ModelMaterials" << YAML::Value << materials;
				out << YAML::EndMap;
			}

			out << YAML::EndMap; // End Entity
		}

		out << YAML::EndSeq; // End Entities
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}
};