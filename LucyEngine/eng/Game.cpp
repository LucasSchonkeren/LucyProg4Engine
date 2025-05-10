#include "Game.h"

#include "Actor.h"

eng::Game::Game() {
	m_RootUptr = std::make_unique<Actor>(*this);
}

eng::Actor& eng::Game::GetRootActor() {
	return *m_RootUptr;
}

void eng::Game::Cleanup() {
	for (eng::Actor* actorPtr : m_DestroyActors) {
		m_NewParentActors.erase(actorPtr);

		actorPtr->GetParent()->DestroyChildActor(actorPtr);
	}

	for (eng::Actor* actorPtr : m_NewParentActors) {
		actorPtr->MoveToNewParent();
	}
}

void eng::Game::FlagActorDestroy(Actor* actor) {
	m_DestroyActors.emplace(actor);
}

void eng::Game::UnFlagActorDestroy(Actor* actor) {
	m_DestroyActors.erase(actor);
}

void eng::Game::FlagActorNewParent(Actor* actor) {
	m_NewParentActors.emplace(actor);
}