#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "../eng/components/TextureRenderer.h"
#include "../eng/components/TextRenderer.h"
#include "../eng/Actor.h"
#include "../eng/components/Transform.h"
#include "../eng/components/FpsTracker.h"

#include "../eng/Engine.h"

#include <chrono>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	// Set up scene root
	eng::Actor f_Root{};

	// Background/logo
	eng::Actor& f_BgActor = f_Root.AddChildActor();
	f_BgActor.AddComponent<cpt::TextureRenderer>().LoadTexture("background.tga");
	eng::Actor& f_LogoActor = f_BgActor.AddChildActor();
	f_LogoActor.AddComponent<cpt::TextureRenderer>().LoadTexture("logo.tga");
	f_LogoActor.AddComponent<cpt::Transform>().SetLocalPosition(216.f, 180.f);

	//Text
	eng::Actor& f_TitleActor = f_Root.AddChildActor();
	f_TitleActor.AddComponent<cpt::TextRenderer>(cpt::TextRenderer{ "Programming 4 Assignment", "Lingua.otf", 36 });
	f_TitleActor.AddComponent<cpt::Transform>().SetLocalPosition(80, 20);

	//Fps tracker
	eng::Actor& f_FpsActor = f_Root.AddChildActor();
	f_FpsActor.AddComponent<cpt::TextRenderer>(cpt::TextRenderer{ "fps: ", "Lingua.otf", 36 });
	f_FpsActor.AddComponent<cpt::Transform>().SetLocalPosition(20, 80);
	f_FpsActor.AddComponent<cpt::FpsTracker>();



	auto& renderer = Renderer::GetInstance();
	auto& input = InputManager::GetInstance();

	// todo: this update loop could use some work.

	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		eng::deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		doContinue = input.ProcessInput();

		f_Root.Update();

		renderer.Render(f_Root);
	}
}
