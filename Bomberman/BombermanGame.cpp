#include "BombermanGame.h"

bmbMan::BombermanGame::BombermanGame() : 
	eng::AbstractGame(std::make_unique<eng::GameTime>(), std::make_unique<eng::ActorGraph>(), std::make_unique<eng::GamePhysics>()) {
	m_Root = std::make_unique<eng::Actor>(*this);
}

eng::Actor& bmbMan::BombermanGame::RootActor() {
	return *m_Root;
}
