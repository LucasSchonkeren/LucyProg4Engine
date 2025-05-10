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
#include "../dae/Renderer.h"
#include "../eng/engine/Scenegraph.h"

#include "../eng/components/TextureRenderer.h"
#include "../eng/components/TextRenderer.h"
#include "../eng/Actor.h"
#include "../eng/components/Transform.h"
#include "../eng/components/FpsTracker.h"
#include "../eng/components/ResourceDisplayLogic.h"
#include "../eng/engine/Time.h"
#include "../eng/engine/Resources.h"
#include "../eng/input/Input.h"
#include "../eng/engine/AudioPlayer.h"
#include "../eng/engine/ConsoleLogger.h"

#include "../eng/engine/Services.h"

#include "../eng/commands/Move.h"
#include "../eng/commands/ModifyActorResource.h"
#include <queue>

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

	dae::Renderer::GetInstance().Init(g_window);
}

eng::Engine::~Engine()
{
	dae::Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void eng::Engine::Run(std::function<u_ptr<Game>()> loadGame) {
	eng::resources::Init();

	auto f_GameUptr = loadGame();

	// Set up scene root
	eng::Actor& f_Root{f_GameUptr->GetRootActor()};

	// Set up services
	auto& renderer = dae::Renderer::GetInstance(); //TODO: make service

	eng::ConsoleLogger f_Logger{};
	eng::service::logger.Subscribe(f_Logger);

	eng::LoggingAudioPlayer f_AudioPlayer{};
	eng::service::audioPlayer.Subscribe(f_AudioPlayer);

	// Set up time
	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue)
	{

		eng::time::UpdateDeltaTime(lastTime);
		lastTime = std::chrono::high_resolution_clock::now();

		eng::time::stage = eng::time::Stages::Start;
		f_Root.Start();


		eng::time::stage = eng::time::Stages::Input;
		doContinue = eng::input::ProcessInput();


		eng::time::stage = eng::time::Stages::Update;
		f_Root.Update();
		f_Root.LateUpdate();
			// Todo if needed: Add Fixed Update

		eng::time::stage = eng::time::Stages::Render;
		renderer.Render(f_Root);

		eng::time::stage = eng::time::Stages::Cleanup;
		f_GameUptr->Cleanup();


		eng::time::stage = eng::time::Stages::None;
		std::this_thread::sleep_for(lastTime + std::chrono::nanoseconds(eng::time::MinNanoSecPerFrame()) - std::chrono::high_resolution_clock::now());
	}

	// Free any remainig resources
	eng::resources::ClearAllResources();
}