#include "epch.h"
#include "Layer.h"
#include "Engine/Core/Application/Application.h"

namespace Engine
{
	Layer::Layer(const std::string& name) :
		_DebugName(name)
	{
		_Window = &Application::Get().GetWindow();
	}
	
	Layer::~Layer()
	{
	}
}
