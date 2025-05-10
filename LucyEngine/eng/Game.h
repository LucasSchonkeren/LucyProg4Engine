#pragma once
#include "utils/utils.h"

namespace eng {

class Actor;

class Game final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Game();
	~Game()	= default;

	Game				(const Game&)	= delete;
	Game& operator=		(const Game&)	= delete;

	Game				(Game&&) = delete;
	Game& operator=		(Game&&) = delete;

public: //---------------------|Getters|-----------------------------------

	Actor& GetRootActor();

public: //---------------------|Gameloop methods|-----------------------------------

	void Cleanup();

public: //---------------------|Scenegraph methods|-----------------------------------

	void FlagActorDestroy(Actor* actor);
	void UnFlagActorDestroy(Actor* actor);
	void FlagActorNewParent(Actor* actor);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Scenegraph fields|----------------------------

	u_ptr<Actor> m_RootUptr{};

	std::set<Actor*> m_DestroyActors{};
	std::set<Actor*> m_NewParentActors{};

}; // !Game

} // !eng