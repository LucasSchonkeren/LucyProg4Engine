#include "Game.h"

#include "Actor.h"

eng::Game::Game(int desiredFps) : 
	m_MinMillisePerFrame{ 1'000 / desiredFps} {
	m_RootUptr = std::make_unique<Actor>(*this);
}

eng::Actor& eng::Game::RootActor() {
	return *m_RootUptr;
}

void eng::Game::Cleanup() {
	for (eng::Actor* actorPtr : m_DestroyActors) {
		m_NewParentActors.erase(actorPtr);

		actorPtr->Disable();
		actorPtr->GetParent()->EraseChildActor(actorPtr);
	}

	for (eng::Actor* actorPtr : m_NewParentActors) {
		actorPtr->Disable();
		actorPtr->MoveToNewParent();
		actorPtr->Enable();
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

double eng::Game::DeltaTime() const {
	return m_DeltaTime;
}

void eng::Game::UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous) {
	m_DeltaTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - previous).count();
}

int eng::Game::MinMilliSecPerFrame() const
{
	return m_MinMillisePerFrame;
}
