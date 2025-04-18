#pragma once

#include "Engine/Core/Application/Application.h"
#include "Engine/Core/Application/EntiryPoint.h"

#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/CollisionEvent.h"

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
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/ECS/Components.h"
#include "Engine/Scene/ECS/Systems.h"
#include "Engine/Scene/ECS/Entity.h"

#include "Engine/ActorClasses/IActor.h"
#include "Engine/ActorClasses/IPawn.h"
#include "Engine/ActorClasses/ICharacter.h"

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
typedef Engine::PhysicsSystem EnginePhysicsSystem;

typedef Engine::MeshComponent EngineStaticMeshComponent;
typedef Engine::NameComponent EngineNameComponent;
typedef Engine::TransformComponent EngineTransformComponent;

typedef Engine::Entity EngineEntity;
typedef Engine::IActor EngineActor;
typedef Engine::IPawn EnginePawn;
typedef Engine::ICharacter EngineCharacter;

typedef Engine::ETextureType EngineTextureType;
typedef Engine::Material EngineMaterial;

#define EngineLoadStaticMesh(path) std::unique_ptr<EngineModel>(new EngineModel(path))
#define EngineCreateShader(vertexPath, fragmentPath) std::shared_ptr<EngineShader>(new EngineShader(vertexPath, fragmentPath))

#define EngineKeyPressed(key) EngineInput::IsKeyPressed(key)
//#define EngineLoadScene(path, scene) SceneManager::LoadScene(path, scene);
//#define EngineSaveScene(path, scene) SceneManager::SaveScene(path, scene);

#define EngineClear() EngineRenderCommand::Clear()
#define EngineSetClearColor(r, g, b, a) EngineRenderCommand::SetClearColor({r, g, b, a})
#define EngineSetPollyMode(face, mode) EngineRenderCommand::SetPolyMode(face, mode);

#define RendererBeginScene(camera) EngineRenderer::BeginScene(camera)
#define RenderShadow(shader, vertexArray, lightSpaceMatrix) EngineRenderer::RenderShadowMap(shader, vertexArray, lightSpaceMatrix)
#define RendererEndScene() EngineRenderer::EndScene()
#define RendererSubmit(shader, vertexArray) EngineRenderer::Submit(shader, vertexArray)

//class SceneManager {
//public:
//	static bool LoadScene(const std::string& path, EngineScene& scene) {
//		Engine::SceneSerializer serializer(scene);
//		return serializer.Deserialize(path);
//	}
//
//	static void SaveScene(const std::string& path, EngineScene& scene) {
//		Engine::SceneSerializer serializer(scene);
//		serializer.Serialize(path);
//	}
//};