#include "Engine.h"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include <functional>

#include "../eng/components/TextureRenderer.h"
#include "../eng/components/TextRenderer.h"
#include "../eng/Actor.h"
#include "../eng/components/Transform.h"
#include "../eng/components/FpsTracker.h"
#include "../eng/components/ResourceDisplayLogic.h"
#include "../eng/engine/Resources.h"
#include "../eng/input/Input.h"
#include "../eng/engine/AudioPlayer.h"
#include "../eng/engine/ConsoleLogger.h"

#include "../eng/engine/Services.h"

#include "../eng/commands/Move.h"
#include "../eng/commands/ModifyActorResource.h"
#include <queue>
#include "engine/Renderer.h"

static SDL_Window* g_window{};

static void PrintSDLVersion()
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

eng::Engine::Engine(const std::string&)
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

	eng::service::renderer.Register(std::make_unique<eng::Renderer>(g_window));
}

eng::Engine::~Engine()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void eng::Engine::Run(std::function<u_ptr<AbstractGame>()> loadGame) {

	auto f_GameUptr = loadGame();

	// Set up services
	auto& renderer = eng::service::renderer.Get(); //TODO: make service

	auto& f_Input{ eng::service::input.Get() };

	// Set up time
	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue)
	{
		eng::Actor& f_Root{ f_GameUptr->RootActor() };

		f_GameUptr->Time().UpdateDeltaTime(lastTime);
		lastTime = std::chrono::high_resolution_clock::now();

		f_Root.Start();

		doContinue = f_Input.ProcessInput();

		f_Root.Update();

		f_GameUptr->Physics().NotifyCollisions();

		f_Root.LateUpdate();

		renderer.Render(f_Root);

		f_GameUptr->ActorGraph().Cleanup();

		std::this_thread::sleep_for(lastTime + std::chrono::milliseconds(f_GameUptr->Time().MinMilliSecPerFrame()) - std::chrono::high_resolution_clock::now());
	}
}