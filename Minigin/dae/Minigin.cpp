#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include "Minigin.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "../eng/engine/Scenegraph.h"

#include "../eng/components/TextureRenderer.h"
#include "../eng/components/TextRenderer.h"
#include "../eng/Actor.h"
#include "../eng/components/Transform.h"
#include "../eng/components/FpsTracker.h"
#include "../game/components/Rotator.h"

#include "../eng/engine/Time.h"
#include "../eng/engine/Resources.h"
#include "../eng/input/InputProcessor.h"

#include "../game/Components/TrashCache.h"
#include "../eng/commands/MoveCommand.h"

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

void dae::Minigin::Run()
{
	eng::time::stage = eng::time::Stages::None;

	// Set up scene root
	eng::Actor f_Root{};

	// Background/logo
	eng::Actor& f_BgActor = f_Root.AddChildActor();
	f_BgActor.AddComponent<cpt::TextureRenderer>().LoadTexture("background.tga");

	eng::Actor& f_LogoActor = f_BgActor.AddChildActor();
	f_LogoActor.AddComponent<cpt::TextureRenderer>().LoadTexture("logo.tga");
	f_LogoActor.GetTransform().SetLocalPosition(216.f, 180.f);

	//Text
	eng::Actor& f_TitleActor = f_Root.AddChildActor();
	f_TitleActor.AddComponent<cpt::TextRenderer>( std::string("Programming 4 Assignment"), std::string("Lingua.otf"), 36u);

	f_TitleActor.GetTransform().SetLocalPosition(80, 20);

	//Fps tracker
	eng::Actor& f_FpsActor = f_Root.AddChildActor();
	f_FpsActor.AddComponent<cpt::TextRenderer>( "fps: ", "Lingua.otf", 36 );
	f_FpsActor.GetTransform().SetLocalPosition(20, 80);
	f_FpsActor.AddComponent<cpt::FpsTracker>();


	////Rotating sprites
	//eng::Actor& f_RotationCenter = f_Root.AddChildActor();
	//f_RotationCenter.GetTransform().SetLocalPosition(300, 300);

	//eng::Actor& f_GuyStandingActor = f_RotationCenter.AddChildActor();
	//f_GuyStandingActor.AddComponent<cpt::TextureRenderer>("BomberManStanding.png");
	//f_GuyStandingActor.AddComponent<cpt::Rotator>(65.f, 2.f);

	//eng::Actor& f_BombActor = f_GuyStandingActor.AddChildActor();
	//f_BombActor.AddComponent<cpt::TextureRenderer>("BigBomb.png");
	//f_BombActor.AddComponent<cpt::Rotator>(30.f, 3.f);

	//eng::Actor& f_TrashCacheActor = f_Root.AddChildActor();
	//f_TrashCacheActor.AddComponent<cpt::TrashCacheImgui1>();
	//f_TrashCacheActor.AddComponent<cpt::TrashCacheImgui2>();

	
	// Move sprites
	eng::Actor& f_GuyStandingActor = f_Root.AddChildActor();
	f_GuyStandingActor.AddComponent<cpt::TextureRenderer>("BomberManStanding.png");
	f_GuyStandingActor.GetTransform().SetLocalPosition(100,400);

	eng::Actor& f_BombActor = f_Root.AddChildActor();
	f_BombActor.AddComponent<cpt::TextureRenderer>("BigBomb.png");
	f_BombActor.GetTransform().SetLocalPosition(150, 400);


	auto& renderer = Renderer::GetInstance();

	int f_Speed{ 50 };

	eng::input::CommandBindings p1Binds{&f_GuyStandingActor};

	p1Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveUpCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::W, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, -f_Speed });

	p1Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveDownCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::S, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, f_Speed });

	p1Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveLeftCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::A, eng::input::KeyPhase::Pressed },
		glm::vec2{ -f_Speed, 0 });

	p1Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveRightCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::D, eng::input::KeyPhase::Pressed },
		glm::vec2{ f_Speed, 0 });

	eng::input::CommandBindings p2Binds{&f_BombActor};

	p2Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveUpCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Up, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, -2 * f_Speed });

	p2Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveDownCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Down, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, 2 * f_Speed });

	p2Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveLeftCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Left, eng::input::KeyPhase::Pressed },
		glm::vec2{ -2 * f_Speed, 0 });

	p2Binds.NewCommand<eng::cmd::MoveCommand>(
		std::string{ "MoveRightCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Right, eng::input::KeyPhase::Pressed },
		glm::vec2{ 2 * f_Speed, 0 });

	eng::input::InputProcessor processInput{ std::vector<eng::input::CommandBindings*>{&p1Binds,& p2Binds} };

	eng::resources::Init();

	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue)
	{

		eng::time::UpdateDeltaTime(lastTime);
		lastTime = std::chrono::high_resolution_clock::now();

		eng::time::stage = eng::time::Stages::Start;
		f_Root.Start();


		eng::time::stage = eng::time::Stages::Input;
		doContinue = processInput();


		eng::time::stage = eng::time::Stages::Update;
		f_Root.Update();
		f_Root.LateUpdate();
			// Todo if needed: Add Fixed Update

		eng::time::stage = eng::time::Stages::Render;
		renderer.Render(f_Root);

		eng::time::stage = eng::time::Stages::Cleanup;
		eng::scenegraph::Cleanup(f_Root);


		eng::time::stage = eng::time::Stages::None;
		std::this_thread::sleep_for(lastTime + std::chrono::nanoseconds(eng::time::MinNanoSecPerFrame()) - std::chrono::high_resolution_clock::now());
	}

	// Free any remainig resources
	eng::resources::ClearAllResources();
}
