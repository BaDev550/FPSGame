#include "epch.h"
//#include "SceneSerializer.h"
//#include "ECS/PhysicsSystem/PhysicsSystem.h"
//#include <fstream>
//
//namespace Engine {
//
//	bool SceneSerializer::Serialize(const std::string& filepath)
//	{
//		nlohmann::json sceneJson;
//		nlohmann::json entitiesJson = nlohmann::json::array();
//
//		auto view = m_Scene.GetRegistry().view<TransformComponent>();
//		for (auto entityHandle : view) {
//			Entity entity(entityHandle);
//
//			if (entity.HasComponent<PawnComponent>() ||
//				entity.HasComponent<ActorComponent>() ||
//				entity.HasComponent<CameraComponent>()) {
//				continue;
//			}
//
//			nlohmann::json entityJson;
//			SerializeEntity(entityJson, entity);
//			entitiesJson.push_back(entityJson);
//		}
//
//		sceneJson["entities"] = entitiesJson;
//
//		try {
//			std::ofstream file(filepath);
//			if (!file.is_open()) return false;
//
//			file << sceneJson.dump(4);
//			return true;
//		}
//		catch (const std::exception& e) {
//			std::cout << "Failed to save scene: " << e.what() << std::endl;
//			return false;
//		}
//	}
//
//	bool SceneSerializer::Deserialize(const std::string& filepath)
//	{
//		try {
//			std::ifstream file(filepath);
//			if (!file.is_open()) return false;
//
//			nlohmann::json sceneJson = nlohmann::json::parse(file);
//
//			auto view = m_Scene.GetRegistry().view<TransformComponent>();
//			for (auto entityHandle : view) {
//				Entity entity(entityHandle);
//				if (!entity.HasComponent<PawnComponent>() &&
//					!entity.HasComponent<ActorComponent>() &&
//					!entity.HasComponent<CameraComponent>()) {
//					entity.DestroyEntity();
//				}
//			}
//
//			for (const auto& entityJson : sceneJson["entities"]) {
//				DeserializeEntity(entityJson);
//			}
//
//			return true;
//		}
//		catch (const std::exception& e) {
//			std::cout << "Failed to load scene: " << e.what() << std::endl;
//			return false;
//		}
//	}
//
//	void SceneSerializer::SerializeEntity(nlohmann::json& json, Entity entity)
//	{
//		if (entity.HasComponent<NameComponent>()) {
//			json["name"] = entity.GetComponent<NameComponent>().name;
//		}
//
//		if (entity.HasComponent<TransformComponent>()) {
//			json["transform"] = entity.GetComponent<TransformComponent>();
//		}
//
//		// Mesh component
//		if (entity.HasComponent<MeshComponent>()) {
//			auto& mesh = entity.GetComponent<MeshComponent>();
//			nlohmann::json meshJson;
//
//			meshJson["model_path"] = mesh._Model ? mesh._Model->GetPath() : "";
//
//			nlohmann::json materialsJson = nlohmann::json::array();
//			for (auto& material : mesh.GetMaterials()) {
//				materialsJson.push_back(*material);
//			}
//			meshJson["materials"] = materialsJson;
//
//			json["mesh"] = meshJson;
//		}
//
//		// Physics components
//		if (entity.HasComponent<BoxColliderComponent>()) {
//			json["box_collider"] = entity.GetComponent<BoxColliderComponent>();
//		}
//
//		if (entity.HasComponent<RigidBodyComponent>()) {
//			json["rigid_body"] = entity.GetComponent<RigidBodyComponent>();
//		}
//
//		// Hierarchy
//		if (entity.GetChildren().size() > 0) {
//			nlohmann::json childrenJson = nlohmann::json::array();
//			for (auto child : entity.GetChildren()) {
//				nlohmann::json childJson;
//				childJson["id"] = child->GetEntityID();
//				childrenJson.push_back(childJson);
//			}
//			json["children"] = childrenJson;
//		}
//	}
//
//	Engine::Entity SceneSerializer::DeserializeEntity(const nlohmann::json& json)
//	{
//		Entity entity = m_Scene.CreateEntity();
//
//		// Basic components
//		if (json.contains("name")) {
//			entity.AddComponent<NameComponent>().name = json["name"].get<std::string>();
//		}
//
//		if (json.contains("transform")) {
//			auto& transform = entity.AddComponent<TransformComponent>();
//			json["transform"].get_to(transform);
//		}
//
//		// Mesh component
//		if (json.contains("mesh")) {
//			auto& meshJson = json["mesh"];
//			std::string modelPath = meshJson["model_path"].get<std::string>();
//
//			std::vector<std::shared_ptr<Material>> materials;
//			if (meshJson.contains("materials")) {
//				for (const auto& matJson : meshJson["materials"]) {
//					Material mat;
//					matJson.get_to(mat);
//					materials.push_back(std::make_shared<Material>(mat));
//				}
//			}
//
//			auto& mesh = entity.AddComponent<MeshComponent>(modelPath);
//			mesh.SetMaterials(materials);
//		}
//
//		// Physics components
//		if (json.contains("box_collider")) {
//			auto& collider = entity.AddComponent<BoxColliderComponent>();
//			json["box_collider"].get_to(collider);
//
//			if (json.contains("rigid_body")) {
//				auto& rb = entity.AddComponent<RigidBodyComponent>();
//				json["rigid_body"].get_to(rb);
//
//				auto& transform = entity.GetComponent<TransformComponent>();
//				PhysicsSystem::Get().AddBoxBody(
//					transform.Position,
//					collider.Size,
//					collider.IsDynamic,
//					entity.GetEntityID()
//				);
//			}
//		}
//		return entity;
//	}
//
//	void SceneSerializer::SerializeHierarchy(nlohmann::json& json, Entity entity)
//	{
//
//	}
//
//	void SceneSerializer::DeserializeHierarchy(const nlohmann::json& json, Entity entity)
//	{
//
//	}
//
//}