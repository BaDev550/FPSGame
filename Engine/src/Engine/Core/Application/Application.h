#pragma once

#include "Engine/Window/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h"
#include "Engine/Window/LayerStack.h"
#include "Engine/Window/ImGui/ImGuiLayer.h"
#include "Engine/Renderer/Model.h"

#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>
#include "yaml-cpp/yaml.h"

namespace YAML {
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

	inline Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
}

namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
	
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_Window; }
	
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void LoadScene(const std::string& path, Scene& scene);
		void SaveScene(const std::string& path, Scene& scene);

		void Close();
	private:
		void Render();
	
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> _Window;
	
		static Application* s_Instance;
		bool _bRunning;
	private:
		ImGuiLayer* _ImGuiLayer;
		LayerStack _LayerStack;
	};

	Application* CreateApplication();
}
