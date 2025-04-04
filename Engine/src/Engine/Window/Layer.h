#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Window/Window.h"

namespace Engine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Example_Layer");
		virtual ~Layer();
	
		virtual void OnAttach() {}
		virtual void OnDeatach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline void SetWindow(Window* window) { _Window = window; }

		inline const std::string& GetName() const { return _DebugName; }
	protected:
		std::string _DebugName;
		Window* _Window;
	};
}