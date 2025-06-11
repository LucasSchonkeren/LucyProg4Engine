#pragma once
#include "utils/utils.h"
#include <chrono>

namespace eng {

class Actor;

class Game final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Game(int desiredFps = 100);
	~Game()	= default;

	Game				(const Game&)	= delete;
	Game& operator=		(const Game&)	= delete;

	Game				(Game&&) = delete;
	Game& operator=		(Game&&) = delete;

public: //---------------------|Gameloop methods|-----------------------------------

	void Cleanup();

public: //---------------------|Scenegraph methods|-----------------------------------

	Actor& RootActor();

	void FlagActorDestroy(Actor* actor);
	void UnFlagActorDestroy(Actor* actor);
	void FlagActorNewParent(Actor* actor);

public: //---------------------|Time methods|-----------------------------------

	double	DeltaTime() const;
	void	UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous);
	int  MinMilliSecPerFrame() const;


private: //---------------------------|Scenegraph fields|----------------------------

	u_ptr<Actor> m_RootUptr{};

	std::set<Actor*> m_DestroyActors{};
	std::set<Actor*> m_NewParentActors{};

private: //---------------------------|time fields|-----------------------------

	double m_DeltaTime{};
	int m_MinMillisePerFrame;

}; // !Game

} // !eng