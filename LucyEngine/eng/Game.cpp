#include "Game.h"

eng::Game::Game(u_ptr<Actor> rootUptr) : 
	m_RootUptr{std::move(rootUptr)} {

}

eng::Actor& eng::Game::GetRootActor() {
	return *m_RootUptr;
}
