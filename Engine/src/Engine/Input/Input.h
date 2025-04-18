#pragma once

#include "Engine/Core/Core.h"

namespace Engine
{
	class Input {
	public:
		static bool IsKeyPressed(int keycode);
	
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	private:
		static Input* s_Instance;
	};
}