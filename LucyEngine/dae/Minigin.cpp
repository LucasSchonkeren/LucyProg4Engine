//#include <stdexcept>
//#define WIN32_LEAN_AND_MEAN 
//#include <windows.h>
//#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//#include <chrono>
//#include <thread>
//#include "Minigin.h"
//#include "InputManager.h"
//#include "Renderer.h"
//#include "ResourceManager.h"
//#include "../eng/engine/Scenegraph.h"
//
//#include "../eng/components/TextureRenderer.h"
//#include "../eng/components/TextRenderer.h"
//#include "../eng/Actor.h"
//#include "../eng/components/Transform.h"
//#include "../eng/components/FpsTracker.h"
//#include "../eng/components/ResourceDisplayLogic.h"
//#include "../game/components/Rotator.h"
//
//#include "../eng/engine/Time.h"
//#include "../eng/engine/Resources.h"
//#include "../eng/input/Input.h"
//
//#include "../game/Components/TrashCache.h"
//#include "../eng/commands/Move.h"
//#include "../eng/commands/ModifyActorResource.h"
//
//static SDL_Window* g_window{};
//
//static void PrintSDLVersion()
//{
//	SDL_version version{};
//	SDL_VERSION(&version);
//	printf("We compiled against SDL version %u.%u.%u ...\n",
//		version.major, version.minor, version.patch);
//
//	SDL_GetVersion(&version);
//	printf("We are linking against SDL version %u.%u.%u.\n",
//		version.major, version.minor, version.patch);
//
//	SDL_IMAGE_VERSION(&version);
//	printf("We compiled against SDL_image version %u.%u.%u ...\n",
//		version.major, version.minor, version.patch);
//
//	version = *IMG_Linked_Version();
//	printf("We are linking against SDL_image version %u.%u.%u.\n",
//		version.major, version.minor, version.patch);
//
//	SDL_TTF_VERSION(&version)
//	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
//		version.major, version.minor, version.patch);
//
//	version = *TTF_Linked_Version();
//	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
//		version.major, version.minor, version.patch);
//}
//
//dae::Minigin::Minigin(const std::string &dataPath)
//{
//	PrintSDLVersion();
//	
//	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
//	{
//		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
//	}
//
//	g_window = SDL_CreateWindow(
//		"Programming 4 assignment",
//		SDL_WINDOWPOS_CENTERED,
//		SDL_WINDOWPOS_CENTERED,
//		640,
//		480,
//		SDL_WINDOW_OPENGL
//	);
//	if (g_window == nullptr) 
//	{
//		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
//	}
//
//	Renderer::GetInstance().Init(g_window);
//
//	ResourceManager::GetInstance().Init(dataPath);
//}
//
//dae::Minigin::~Minigin()
//{
//	Renderer::GetInstance().Destroy();
//	SDL_DestroyWindow(g_window);
//	g_window = nullptr;
//	SDL_Quit();
//}
//
//void dae::Minigin::Run()
//{
//
//	// Set up scene root
//	eng::Actor f_Root{};
//
//	// Background/logo
//	eng::Actor& f_BgActor = f_Root.AddChildActor();
//	f_BgActor.AddComponent<eng::cpt::TextureRenderer>().LoadTexture("background.tga");
//
//	eng::Actor& f_LogoActor = f_BgActor.AddChildActor();
//	f_LogoActor.AddComponent<eng::cpt::TextureRenderer>().LoadTexture("logo.tga");
//	f_LogoActor.GetTransform().SetLocalPosition(216.f, 220.f);
//
//	//Text
//	eng::Actor& f_TitleActor = f_Root.AddChildActor();
//	f_TitleActor.AddComponent<eng::cpt::TextRenderer>( std::string("Programming 4 Assignment"), std::string("Lingua.otf"), 36u);
//	f_TitleActor.GetTransform().SetLocalPosition(80, 20);
//
//	eng::Actor& f_P1ControlsTextActor{ f_Root.AddChildActor() };
//	f_P1ControlsTextActor.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player: WASD to move, Q to lose lives, E or SPACEBAR to gain points"), std::string("Lingua.otf"), 17u);
//	f_P1ControlsTextActor.GetTransform().SetLocalPosition(10, 60);
//
//	eng::Actor& f_P2ControlsTextActor{ f_Root.AddChildActor() };
//	f_P2ControlsTextActor.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player: D-pad to move, A to lose lives, B or Y to gain points"), std::string("Lingua.otf"), 17u);
//	f_P2ControlsTextActor.GetTransform().SetLocalPosition(10, 78);
//
//	eng::Actor& f_P1LifeCounter{ f_Root.AddChildActor() };
//	f_P1LifeCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player lives: 3"), std::string("Lingua.otf"), 24u);
//	f_P1LifeCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Keyboard player lives: {hp}");
//	f_P1LifeCounter.GetTransform().SetLocalPosition(10, 100);
//
//	eng::Actor& f_P1ScoreCounter{ f_Root.AddChildActor() };
//	f_P1ScoreCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player Score: 0"), std::string("Lingua.otf"), 24u);
//	f_P1ScoreCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Keyboard player Score: {score}");
//
//	f_P1ScoreCounter.GetTransform().SetLocalPosition(10, 125);
//
//	eng::Actor& f_P2LifeCounter{ f_Root.AddChildActor() };
//	f_P2LifeCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player lives: 3"), std::string("Lingua.otf"), 24u);
//	f_P2LifeCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Gamepad player lives: {hp}");
//
//	f_P2LifeCounter.GetTransform().SetLocalPosition(10, 150);
//
//	eng::Actor& f_P2ScoreCounter{ f_Root.AddChildActor() };
//	f_P2ScoreCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player Score: 0"), std::string("Lingua.otf"), 24u);
//	f_P2ScoreCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Gamepad player Score: {score}");
//
//	f_P2ScoreCounter.GetTransform().SetLocalPosition(10, 175);
//
//	//Fps tracker
//	eng::Actor& f_FpsActor = f_Root.AddChildActor();
//	f_FpsActor.AddComponent<eng::cpt::TextRenderer>( "fps: ", "Lingua.otf", 16u );
//	f_FpsActor.GetTransform().SetLocalPosition(10, 10);
//	f_FpsActor.AddComponent<eng::cpt::FpsTracker>();
//
//
//	////Rotating sprites
//	//eng::Actor& f_RotationCenter = f_Root.AddChildActor();
//	//f_RotationCenter.GetTransform().SetLocalPosition(300, 300);
//
//	//eng::Actor& f_GuyStandingActor = f_RotationCenter.AddChildActor();
//	//f_GuyStandingActor.AddComponent<cpt::TextureRenderer>("BomberManStanding.png");
//	//f_GuyStandingActor.AddComponent<cpt::Rotator>(65.f, 2.f);
//
//	//eng::Actor& f_BombActor = f_GuyStandingActor.AddChildActor();
//	//f_BombActor.AddComponent<cpt::TextureRenderer>("BigBomb.png");
//	//f_BombActor.AddComponent<cpt::Rotator>(30.f, 3.f);
//
//	//eng::Actor& f_TrashCacheActor = f_Root.AddChildActor();
//	//f_TrashCacheActor.AddComponent<cpt::TrashCacheImgui1>();
//	//f_TrashCacheActor.AddComponent<cpt::TrashCacheImgui2>();
//
//	
//	// Move sprites
//	eng::Actor& f_GuyStandingActor = f_Root.AddChildActor();
//	f_GuyStandingActor.AddComponent<eng::cpt::TextureRenderer>("BomberManStanding.png");
//	auto& f_GuyStandingResources{ f_GuyStandingActor.AddComponent<eng::cpt::ResourceTracker>() };
//	f_GuyStandingResources.NewResource("hp", 5u);
//	f_GuyStandingResources.NewResource("score", 0u, true);
//	f_GuyStandingResources.AddObserver(*f_P1LifeCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
//	f_GuyStandingResources.AddObserver(*f_P1ScoreCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
//	f_GuyStandingActor.GetTransform().SetLocalPosition(100,400);
//
//	eng::Actor& f_BombActor = f_Root.AddChildActor();
//	f_BombActor.AddComponent<eng::cpt::TextureRenderer>("BigBomb.png");
//	auto& f_BombActorResources{ f_BombActor.AddComponent<eng::cpt::ResourceTracker>() };
//	f_BombActorResources.NewResource("hp", 3u);
//	f_BombActorResources.NewResource("score", 0u, true);
//	f_BombActorResources.AddObserver(*f_P2LifeCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
//	f_BombActorResources.AddObserver(*f_P2ScoreCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
//	f_BombActor.GetTransform().SetLocalPosition(150, 400);
//
//
//
//	int f_Speed{ 100 };
//
//	eng::input::CommandBindings p1Binds{&f_GuyStandingActor};
//
//	p1Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveUpCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::W, eng::input::KeyPhase::Pressed },
//		glm::vec2{ 0, -f_Speed });
//
//	p1Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveDownCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::S, eng::input::KeyPhase::Pressed },
//		glm::vec2{ 0, f_Speed });
//
//	p1Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveLeftCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::A, eng::input::KeyPhase::Pressed },
//		glm::vec2{ -f_Speed, 0 });
//
//	p1Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveRightCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::D, eng::input::KeyPhase::Pressed },
//		glm::vec2{ f_Speed, 0 });
//
//	p1Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{"LoseALife"},
//		eng::input::Keystate{ eng::input::KeyboardKeys::Q, eng::input::KeyPhase::Down },
//		"hp", -1
//	);
//
//	p1Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{ "gainSomeScore" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::E, eng::input::KeyPhase::Down },
//		"score", 10
//	);
//
//	p1Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{ "gainALotOfScore" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::Space, eng::input::KeyPhase::Down },
//		"score", 100
//	);
//
//	eng::input::CommandBindings p2Binds{&f_BombActor};
//
//	p2Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveUpCommand" },
//		eng::input::Keystate{ eng::input::GamepadKeys::Up, eng::input::KeyPhase::Pressed },
//		glm::vec2{ 0, -2 * f_Speed });
//
//	p2Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveDownCommand" },
//		eng::input::Keystate{ eng::input::GamepadKeys::Down, eng::input::KeyPhase::Pressed },
//		glm::vec2{ 0, 2 * f_Speed });
//
//	p2Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveLeftCommand" },
//		eng::input::Keystate{ eng::input::GamepadKeys::Left, eng::input::KeyPhase::Pressed },
//		glm::vec2{ -2 * f_Speed, 0 });
//
//	p2Binds.NewCommand<eng::cmd::Move>(
//		std::string{ "MoveRightCommand" },
//		eng::input::Keystate{ eng::input::GamepadKeys::Right, eng::input::KeyPhase::Pressed },
//		glm::vec2{ 2 * f_Speed, 0 });
//
//	p2Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{ "LoseALife" },
//		eng::input::Keystate{ eng::input::GamepadKeys::A, eng::input::KeyPhase::Down },
//		"hp", -1
//	);
//
//	p2Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{ "gainSomeScore" },
//		eng::input::Keystate{ eng::input::GamepadKeys::B, eng::input::KeyPhase::Down },
//		"score", 10
//	);
//
//	p2Binds.NewCommand<eng::cmd::ModifyActorResource>(
//		std::string{ "gainALotOfScore" },
//		eng::input::Keystate{ eng::input::GamepadKeys::Y, eng::input::KeyPhase::Down },
//		"score", 100
//	);
//
//	eng::input::RegisterCommandBinding(std::move(p1Binds));
//	eng::input::RegisterCommandBinding(std::move(p2Binds));
//
//	eng::resources::Init();
//	auto& renderer = Renderer::GetInstance();
//
//
//	auto lastTime = std::chrono::high_resolution_clock::now();
//
//	bool doContinue = true;
//	while (doContinue)
//	{
//
//		eng::time::UpdateDeltaTime(lastTime);
//		lastTime = std::chrono::high_resolution_clock::now();
//
//		eng::time::stage = eng::time::Stages::Start;
//		f_Root.Init();
//		f_Root.Start();
//
//
//		eng::time::stage = eng::time::Stages::Input;
//		doContinue = eng::input::ProcessInput();
//
//
//		eng::time::stage = eng::time::Stages::Update;
//		f_Root.Update();
//		f_Root.LateUpdate();
//			// Todo if needed: Add Fixed Update
//
//		eng::time::stage = eng::time::Stages::Render;
//		renderer.Render(f_Root);
//
//		eng::time::stage = eng::time::Stages::Cleanup;
//		eng::scenegraph::Cleanup(f_Root);
//
//
//		eng::time::stage = eng::time::Stages::None;
//		std::this_thread::sleep_for(lastTime + std::chrono::nanoseconds(eng::time::MinNanoSecPerFrame()) - std::chrono::high_resolution_clock::now());
//	}
//
//	// Free any remainig resources
//	eng::resources::ClearAllResources();
//}
