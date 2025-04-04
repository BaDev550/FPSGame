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
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"

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
typedef Engine::Camera EngineCamera;
typedef Engine::Model EngineModel;
typedef Engine::BufferLayout EngineBufferLayout;
typedef Engine::BufferElement EngineBufferElement;
typedef Engine::Vertex EngineVertex;
typedef Engine::ShaderDataType EngineShaderDataType;

#define EngineLoadStaticMesh(path) std::unique_ptr<EngineModel>(new EngineModel(path))
#define EngineCreateShader(vertexPath, fragmentPath) std::unique_ptr<EngineShader>(new EngineShader(vertexPath, fragmentPath))

#define EngineKeyPressed(key) EngineInput::IsKeyPressed(key)

#define EngineClear() EngineRenderCommand::Clear()
#define EngineSetClearColor(r, g, b, a) EngineRenderCommand::SetClearColor({r, g, b, a})
#define EngineSetPollyMode(face, mode) EngineRenderCommand::SetPolyMode(face, mode);

#define RendererBeginScene(camera) EngineRenderer::BeginScene(camera)
#define RendererEndScene() EngineRenderer::EndScene()
#define RendererSubmit(shader, vertexArray) EngineRenderer::Submit(shader, vertexArray)