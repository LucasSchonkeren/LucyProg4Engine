#pragma once
#include <chrono>

#include "../utils/utils.h"
#include "GamePhysics.h"
#include "GameTime.h"
#include "ActorGraph.h"

namespace eng {

class Actor;

class AbstractGame {
public: // Virtual methods
	AbstractGame(u_ptr<IGameTime> time, u_ptr<IActorGraph> graph, u_ptr<IGamePhysics> physics);
	virtual ~AbstractGame()	= default;

	virtual Actor& RootActor() = 0;
	
public: // Services

	IGameTime& Time();
	IActorGraph& ActorGraph();
	IGamePhysics& Physics();

private:
	u_ptr<IGameTime> m_TimeUptr;
	u_ptr<IActorGraph> m_GraphUptr;
	u_ptr<IGamePhysics> m_PhysicsUptr;


}; // !AbstractGame

} // !eng