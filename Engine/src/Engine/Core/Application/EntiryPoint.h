#pragma once

#ifdef E_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

#ifdef DEFINE_MAIN
int main(int argc, char** argv) {
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif

#endif // E_PLATFORM_WINDOWS