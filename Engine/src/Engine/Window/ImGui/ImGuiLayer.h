#pragma once

#include "Engine/Window/Layer.h"

#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
	
		virtual void OnAttach() override;
		virtual void OnDeatach() override;
		virtual void OnImGuiRender() override;
	
		void Begin();
		void End();
	private:
		float _Time = 0.0f;
	};
}
