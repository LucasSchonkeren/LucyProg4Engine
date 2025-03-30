#pragma once
#include "Actor.h"
#include "utils/utils.h"

namespace eng {

class Game final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Game(u_ptr<Actor> rootUptr);
	~Game()	= default;

	Game				(const Game&)	= delete;
	Game& operator=		(const Game&)	= delete;

	Game				(Game&&) = default;
	Game& operator=		(Game&&) = default;

public: //---------------------|Getters|-----------------------------------

	Actor& GetRootActor();

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	u_ptr<Actor> m_RootUptr;

}; // !Game

} // !eng