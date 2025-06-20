#ifndef NOSDL
#include <SDL.h>
#endif


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "eng/Engine.h"
#include "eng/Game/AbstractGame.h"
#include "dae/ResourceManager.h"

#include "eng/utils/utils.h"
#include "eng/components/TextureRenderer.h"
#include "eng/components/FpsTracker.h"
#include "eng/components/ResourceDisplayLogic.h"
#include "eng/components/TextRenderer.h"
#include "eng/components/GridTransform.h"
#include "eng/components/MovementHandler.h"

#include "eng/input/Input.h"
#include "eng/commands/Move.h"
#include "eng/commands/MoveAlongGrid.h"
#include "eng/commands/ModifyActorResource.h"
#include "Components/AudioController.h"
#include <eng/engine/ConsoleLogger.h>
#include <eng/engine/AudioPlayer.h>
#include <eng/engine/Resources.h>
#include "BombermanGame.h"
#include "eng/components/BoxCollider.h"
#include "eng/engine/Renderer.h"
#include <eng/components/SpriteController.h>

#ifndef NOSDL

static u_ptr<eng::AbstractGame> load() {
	// Set up Game and scene root
	u_ptr<eng::AbstractGame> f_GameUptr{std::make_unique<bmbMan::BombermanGame>()};
	eng::Actor& f_Root{ f_GameUptr->RootActor() };

	// Background/logo
	eng::Actor& f_BgActor = f_Root.AddChildActor();
	f_BgActor.AddComponent<eng::cpt::TextureRenderer>("background.tga");

	eng::Actor& f_LogoActor = f_BgActor.AddChildActor();
	f_LogoActor.AddComponent<eng::cpt::TextureRenderer>("logo.tga");
	f_LogoActor.GetTransform().SetLocalPosition(216.f, 220.f);

	//Text
	eng::Actor& f_TitleActor = f_Root.AddChildActor();
	f_TitleActor.AddComponent<eng::cpt::TextRenderer>(std::string("Programming 4 Assignment"), std::string("Lingua.otf"), 36u);
	f_TitleActor.GetTransform().SetLocalPosition(80, 20);

	eng::Actor& f_P1ControlsTextActor{ f_Root.AddChildActor() };
	f_P1ControlsTextActor.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player: WASD to move, Q to lose lives, E to gain points AND play a sound!"), std::string("Lingua.otf"), 17u);
	f_P1ControlsTextActor.GetTransform().SetLocalPosition(10, 60);

	eng::Actor& f_P2ControlsTextActor{ f_Root.AddChildActor() };
	f_P2ControlsTextActor.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player: D-pad to move, A to lose lives, B or Y to gain points"), std::string("Lingua.otf"), 17u);
	f_P2ControlsTextActor.GetTransform().SetLocalPosition(10, 78);

	eng::Actor& f_P1LifeCounter{ f_Root.AddChildActor() };
	f_P1LifeCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player lives: 3"), std::string("Lingua.otf"), 24u);
	f_P1LifeCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Keyboard player lives: {hp}");
	f_P1LifeCounter.GetTransform().SetLocalPosition(10, 100);

	eng::Actor& f_P1ScoreCounter{ f_Root.AddChildActor() };
	f_P1ScoreCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Keyboard player Score: 0"), std::string("Lingua.otf"), 24u);
	f_P1ScoreCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Keyboard player Score: {score}");

	f_P1ScoreCounter.GetTransform().SetLocalPosition(10, 125);

	eng::Actor& f_P2LifeCounter{ f_Root.AddChildActor() };
	f_P2LifeCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player lives: 3"), std::string("Lingua.otf"), 24u);
	f_P2LifeCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Gamepad player lives: {hp}");

	f_P2LifeCounter.GetTransform().SetLocalPosition(10, 150);

	eng::Actor& f_P2ScoreCounter{ f_Root.AddChildActor() };
	f_P2ScoreCounter.AddComponent<eng::cpt::TextRenderer>(std::string("Gamepad player Score: 0"), std::string("Lingua.otf"), 24u);
	f_P2ScoreCounter.AddComponent<eng::cpt::ResourceDisplayLogic>("Gamepad player Score: {score}");

	f_P2ScoreCounter.GetTransform().SetLocalPosition(10, 175);

	//Fps tracker
	eng::Actor& f_FpsActor = f_Root.AddChildActor();
	f_FpsActor.AddComponent<eng::cpt::TextRenderer>("fps: ", "Lingua.otf", 16u);
	f_FpsActor.GetTransform().SetLocalPosition(10, 10);
	f_FpsActor.AddComponent<eng::cpt::FpsTracker>();


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


	// Move actors
	eng::Actor& f_GuyStandingActor = f_Root.AddChildActor();
	f_GuyStandingActor.AddComponent<eng::cpt::TextureRenderer>("Player.png", glm::vec2{48,48});
	auto& f_GuyStandingResources{ f_GuyStandingActor.AddComponent<eng::cpt::ResourceTracker>() };
	f_GuyStandingResources.NewResource("hp", 5u);
	f_GuyStandingResources.NewResource("score", 0u, true);
	f_GuyStandingResources.AddObserver(*f_P1LifeCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
	f_GuyStandingResources.AddObserver(*f_P1ScoreCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
	f_GuyStandingActor.GetTransform().SetLocalPosition(100, 400);
	f_GuyStandingActor.AddComponent<eng::cpt::BoxCollider>(glm::vec2(47, 47), glm::vec2{}, true);
	f_GuyStandingActor.AddComponent<eng::cpt::MovementHandler>(10.f);
	std::unordered_map<std::string, std::vector<glm::ivec2>> f_SourcePos{
		{"WalkLeft", {
			{0,0}, {16,0}, {32,0}
		}}
	};
	f_GuyStandingActor.AddComponent<eng::cpt::SpriteController>(glm::ivec2{16,16}, f_SourcePos, "WalkLeft", 0.2f);

	eng::Actor& f_BombActor = f_Root.AddChildActor();
	f_BombActor.AddComponent<eng::cpt::TextureRenderer>("BigBomb.png");
	auto& f_BombActorResources{ f_BombActor.AddComponent<eng::cpt::ResourceTracker>() };
	f_BombActorResources.NewResource("hp", 3u);
	f_BombActorResources.NewResource("score", 0u, true);
	f_BombActorResources.AddObserver(*f_P2LifeCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
	f_BombActorResources.AddObserver(*f_P2ScoreCounter.GetComponent<eng::cpt::ResourceDisplayLogic>());
	f_BombActor.GetTransform().SetLocalPosition(150, 400);
	f_BombActor.AddComponent<eng::cpt::BoxCollider>(glm::vec2(32, 32), glm::vec2{}, true);
	f_BombActor.AddComponent<eng::cpt::MovementHandler>(0.f);



	// Audio
	eng::Actor& f_AudioActor{ f_Root.AddChildActor() };
	auto& f_AudioComp{ f_AudioActor.AddComponent<eng::cpt::AudioController>() };

	f_GuyStandingResources.AddObserver(f_AudioComp);

	int f_Speed{ 100 };

	u_ptr<eng::input::CommandBindings> p1Binds{ std::make_unique<eng::input::CommandBindings>(&f_GuyStandingActor) };

	p1Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveUpCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::W, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, -f_Speed });

	p1Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveDownCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::S, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, f_Speed });

	p1Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveLeftCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::A, eng::input::KeyPhase::Pressed },
		glm::vec2{ -f_Speed, 0 });

	p1Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveRightCommand" },
		eng::input::Keystate{ eng::input::KeyboardKeys::D, eng::input::KeyPhase::Pressed },
		glm::vec2{ f_Speed, 0 });

	p1Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "LoseALife" },
		eng::input::Keystate{ eng::input::KeyboardKeys::Q, eng::input::KeyPhase::Down },
		"hp", -1
	);

	p1Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "gainSomeScore" },
		eng::input::Keystate{ eng::input::KeyboardKeys::E, eng::input::KeyPhase::Down },
		"score", 10
	);

	p1Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "gainALotOfScore" },
		eng::input::Keystate{ eng::input::KeyboardKeys::Space, eng::input::KeyPhase::Down },
		"score", 100
	);

	u_ptr<eng::input::CommandBindings> p2Binds{ std::make_unique<eng::input::CommandBindings>(& f_BombActor) };

	p2Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveUpCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Up, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, -2 * f_Speed });

	p2Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveDownCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Down, eng::input::KeyPhase::Pressed },
		glm::vec2{ 0, 2 * f_Speed });

	p2Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveLeftCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Left, eng::input::KeyPhase::Pressed },
		glm::vec2{ -2 * f_Speed, 0 });

	p2Binds->NewCommand<eng::cmd::Move>(
		std::string{ "MoveRightCommand" },
		eng::input::Keystate{ eng::input::GamepadKeys::Right, eng::input::KeyPhase::Pressed },
		glm::vec2{ 2 * f_Speed, 0 });

	p2Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "LoseALife" },
		eng::input::Keystate{ eng::input::GamepadKeys::A, eng::input::KeyPhase::Down },
		"hp", -1
	);

	p2Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "gainSomeScore" },
		eng::input::Keystate{ eng::input::GamepadKeys::B, eng::input::KeyPhase::Down },
		"score", 10
	);

	p2Binds->NewCommand<eng::cmd::ModifyActorResource>(
		std::string{ "gainALotOfScore" },
		eng::input::Keystate{ eng::input::GamepadKeys::Y, eng::input::KeyPhase::Down },
		"score", 100
	);

	auto& f_Input{ eng::service::input.Get() };

	f_Input.RegisterCommandBinding(std::move(p1Binds));
	f_Input.RegisterCommandBinding(std::move(p2Binds));


	return std::move(f_GameUptr);
}

//static u_ptr<eng::Game> GridTest() {
//	// Set up Game and scene root
//	u_ptr<eng::Game> f_GameUptr{ std::make_unique<eng::Game>() };
//	eng::Actor& f_Root{ f_GameUptr->GetRootActor() };
//
//	// Background/logo
//	eng::Actor& f_BgActor = f_Root.AddChildActor();
//	f_BgActor.AddComponent<eng::cpt::TextureRenderer>().LoadTexture("Arena.png");
//
//	//Fps tracker
//	eng::Actor& f_FpsActor = f_Root.AddChildActor();
//	f_FpsActor.AddComponent<eng::cpt::TextRenderer>("fps: ", "Lingua.otf", 16u);
//	f_FpsActor.GetTransform().SetLocalPosition(10, 10);
//	f_FpsActor.AddComponent<eng::cpt::FpsTracker>();
//
//	// Grid
//	eng::Actor& f_GridActor = f_Root.AddChildActor();
//	f_GridActor.AddComponent<eng::cpt::Grid>(144.f);
//
//	// Plyer actor
//	eng::Actor& f_GuyStandingActor = f_GridActor.AddChildActor();
//	f_GuyStandingActor.AddComponent<eng::cpt::TextureRenderer>("BomberManStanding.png");
//	f_GuyStandingActor.GetTransform().SetLocalPosition(54, 54);
//	f_GuyStandingActor.AddComponent<eng::cpt::GridTransform>();
//
//	float f_Speed{ 120 };
//
//	u_ptr<eng::input::CommandBindings> p1Binds{ std::make_unique<eng::input::CommandBindings>(&f_GuyStandingActor) };
//
//	p1Binds->NewCommand<eng::cmd::MoveAlongGrid>(
//		std::string{ "MoveUpCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::W, eng::input::KeyPhase::Pressed },
//		 false, -f_Speed, 0.2f );
//
//	p1Binds->NewCommand<eng::cmd::MoveAlongGrid>(
//		std::string{ "MoveDownCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::S, eng::input::KeyPhase::Pressed },
//		false, f_Speed ,0.2f );
//
//	p1Binds->NewCommand<eng::cmd::MoveAlongGrid>(
//		std::string{ "MoveLeftCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::A, eng::input::KeyPhase::Pressed },
//		 true, -f_Speed, 0.2f );
//
//	p1Binds->NewCommand<eng::cmd::MoveAlongGrid>(
//		std::string{ "MoveRightCommand" },
//		eng::input::Keystate{ eng::input::KeyboardKeys::D, eng::input::KeyPhase::Pressed },
//		 true, f_Speed, 0.2f );
//
//
//	eng::input::RegisterCommandBinding(std::move(p1Binds));
//
//	return std::move(f_GameUptr);
//}

int main(int, char* []) {
	eng::service::logger.Register(std::make_unique<eng::ConsoleLogger>());
	eng::service::audioPlayer.Register(std::make_unique<eng::LoggingAudioPlayer>());
	eng::service::resources.Register(std::make_unique<eng::SdlResourceLoader>());
	eng::service::input.Register(std::make_unique<eng::input::Input>());
	

	eng::Engine f_Engine("../Data/");
	f_Engine.Run(load);

	return 0;
}

#endif // !ifndef NOSDL

#ifdef NOSDL

#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../libs/catch.hpp"

TEST_CASE("The Catch2 framework works") {
	REQUIRE(true);
}

#endif
