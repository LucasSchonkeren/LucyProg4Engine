#include "AbstractGame.h"

eng::AbstractGame::AbstractGame(u_ptr<IGameTime> time, u_ptr<IActorGraph> graph, u_ptr<IGamePhysics> physics) :
    m_GraphUptr(std::move(graph)), m_TimeUptr(std::move(time)), m_PhysicsUptr(std::move(physics)) {
}

eng::IGameTime& eng::AbstractGame::Time() {
    return *m_TimeUptr;
}

eng::IActorGraph& eng::AbstractGame::ActorGraph() {
    return *m_GraphUptr;
}

eng::IGamePhysics& eng::AbstractGame::Physics() {
    return *m_PhysicsUptr;
}
