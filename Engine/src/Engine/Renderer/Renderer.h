#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Engine/Window/Window.h"
#include "Engine/Scene/ECS/Components.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init(std::unique_ptr<Window>& window);
		static void BeginScene(CameraComponent& camera);
		static void EndScene();
		
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray);
		static void SubmitInstanced(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instanceCount);
		static void RenderShadowMap(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray, const glm::mat4& lightSpaceMatrix);
		static void RenderFrameBufferScreen(const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API CurrentAPI() { return RendererAPI::GetAPI(); }
		inline static std::shared_ptr<FrameBuffer> GetFramebuffer() { return _FrameBuffer; }
		inline static std::shared_ptr<FrameBuffer> GetShadowbuffer() { return _ShadowMapFramebuffer; }
		inline static std::shared_ptr<Texture2D> GetShadowMap() { return _ShadowMap; }
		inline static std::shared_ptr<Texture2D> GetScreenTexture() { return _ScreenTexture; }

	private:
		struct SSCeneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SSCeneData* _SceneData;
		static std::shared_ptr<FrameBuffer> _FrameBuffer;
		static std::shared_ptr<FrameBuffer> _ShadowMapFramebuffer;
		static std::shared_ptr<Texture2D> _ShadowMap;
		static std::shared_ptr<Texture2D> _ScreenTexture;
	};
}