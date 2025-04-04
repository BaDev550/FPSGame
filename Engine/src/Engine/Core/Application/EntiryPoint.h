#pragma once

#ifdef E_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) {
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif // TU_PLATFORM_WINDOWS

