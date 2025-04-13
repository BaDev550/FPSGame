//#pragma once
//
//#include "nlohmann/json.hpp"
//#include "Scene.h"
//
//#include "ECS/Components.h"
//#include "ECS/Entity.h"
//
//using json = nlohmann::json;
//namespace Engine {
//	class SceneSerializer
//	{
//	public:
//		SceneSerializer(Scene& scene) : m_Scene(scene) {}
//
//		bool Serialize(const std::string& filepath);
//		bool Deserialize(const std::string& filepath);
//
//	private:
//		void SerializeEntity(nlohmann::json& json, Entity entity);
//		Entity DeserializeEntity(const nlohmann::json& json);
//		static void SerializeHierarchy(nlohmann::json& json, Entity entity);
//		static void DeserializeHierarchy(const nlohmann::json& json, Entity entity);
//
//		Scene& m_Scene;
//	};
//
//	namespace json_utils {
//		// glm::vec3 serialization
//		inline void to_json(nlohmann::json& j, const glm::vec3& vec) {
//			j = { vec.x, vec.y, vec.z };
//		}
//
//		inline void from_json(const nlohmann::json& j, glm::vec3& vec) {
//			if (j.is_array() && j.size() == 3) {
//				vec.x = j[0].get<float>();
//				vec.y = j[1].get<float>();
//				vec.z = j[2].get<float>();
//			}
//		}
//
//		// TransformComponent serialization
//		inline void to_json(nlohmann::json& j, const TransformComponent& tc) {
//			j = nlohmann::json{
//				{ "position", tc.Position },
//				{ "rotation", tc.Rotation },
//				{ "scale", tc.Scale }
//			};
//		}
//
//		inline void from_json(const nlohmann::json& j, TransformComponent& tc) {
//			if (j.contains("position")) json_utils::from_json(j["position"], tc.Position);
//			if (j.contains("rotation")) json_utils::from_json(j["rotation"], tc.Rotation);
//			if (j.contains("scale"))    json_utils::from_json(j["scale"], tc.Scale);
//		}
//
//		// ECollisionMode serialization
//		inline void to_json(nlohmann::json& j, const ECollisionMode& mode) {
//			j = (mode == ECollisionMode::Disabled) ? "disabled" : "enabled";
//		}
//
//		inline void from_json(const nlohmann::json& j, ECollisionMode& mode) {
//			mode = (j.get<std::string>() == "disabled") ?
//				ECollisionMode::Disabled : ECollisionMode::Enabled;
//		}
//
//		// BoxColliderComponent serialization
//		inline void to_json(nlohmann::json& j, const BoxColliderComponent& c) {
//			j = {
//				{ "size", c.Size },
//				{ "is_dynamic", c.IsDynamic },
//				{ "mode", c.Mode }
//			};
//		}
//
//		inline void from_json(const nlohmann::json& j, BoxColliderComponent& c) {
//			if (j.contains("size")) j["size"].get_to(c.Size);
//			if (j.contains("is_dynamic")) j["is_dynamic"].get_to(c.IsDynamic);
//			if (j.contains("mode")) j["mode"].get_to(c.Mode);
//		}
//
//		// RigidBodyComponent serialization
//		inline void to_json(nlohmann::json& j, const RigidBodyComponent& rb) {
//			j = { { "mass", rb.Mass } };
//		}
//
//		inline void from_json(const nlohmann::json& j, RigidBodyComponent& rb) {
//			if (j.contains("mass")) j["mass"].get_to(rb.Mass);
//		}
//
//		// ETextureType serialization
//		inline void to_json(nlohmann::json& j, const ETextureType& type) {
//			switch (type) {
//			case ETextureType::Diffuse: j = "diffuse"; break;
//			case ETextureType::Normal: j = "normal"; break;
//			case ETextureType::Specular: j = "specular"; break;
//			case ETextureType::AmbientOcclusion: j = "ambient_occlusion"; break;
//			default: j = "unknown";
//			}
//		}
//
//		inline void from_json(const nlohmann::json& j, ETextureType& type) {
//			static const std::unordered_map<std::string, ETextureType> mapping = {
//				{"diffuse", ETextureType::Diffuse},
//				{"normal", ETextureType::Normal},
//				{"specular", ETextureType::Specular},
//				{"ambient_occlusion", ETextureType::AmbientOcclusion}
//			};
//
//			auto str = j.get<std::string>();
//			type = mapping.count(str) ? mapping.at(str) : ETextureType::Diffuse;
//		}
//
//		// Material serialization
//		inline void to_json(nlohmann::json& j, const Material& mat) {
//			j = {
//				{"highlight", mat._bHighlight},
//				{"metalness", mat._metalness},
//				{"roughness", mat._roughness},
//				{"shininess", mat._shininess},
//				{"textures", {}}
//			};
//
//			for (const auto& [type, tex] : mat.GetAllTextures()) {
//				if (tex) {
//					j["textures"][std::to_string(static_cast<int>(type))] = tex->GetFile();
//				}
//			}
//		}
//
//		inline void from_json(const nlohmann::json& j, Material& mat) {
//			if (j.contains("highlight")) j["highlight"].get_to(mat._bHighlight);
//			if (j.contains("metalness")) j["metalness"].get_to(mat._metalness);
//			if (j.contains("roughness")) j["roughness"].get_to(mat._roughness);
//			if (j.contains("shininess")) j["shininess"].get_to(mat._shininess);
//
//			if (j.contains("textures")) {
//				for (const auto& [key, value] : j["textures"].items()) {
//					try {
//						ETextureType type = static_cast<ETextureType>(std::stoi(key));
//						std::string path = value.get<std::string>();
//						std::string strType;
//						switch (type)
//						{
//						case Engine::ETextureType::Diffuse: strType = "texture_diffuse"; break;
//						case Engine::ETextureType::Specular:strType = "texture_specular"; break;
//						case Engine::ETextureType::Normal:  strType = "texture_normal"; break;
//						case Engine::ETextureType::AmbientOcclusion: strType = "texture_ao"; break;
//						}
//						if (!path.empty()) {
//							mat.AddTexture(type, Texture2D::Create(path, strType));
//						}
//					}
//					catch (...) {
//					}
//				}
//			}
//		}
//	}
//}