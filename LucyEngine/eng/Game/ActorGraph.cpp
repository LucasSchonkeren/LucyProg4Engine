#include "ActorGraph.h"
#include "../Actor.h"

void eng::ActorGraph::Cleanup() {
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

void eng::ActorGraph::FlagActorDestroy(Actor* actor) {
	m_DestroyActors.emplace(actor);
}

void eng::ActorGraph::UnFlagActorDestroy(Actor* actor) {
	m_DestroyActors.erase(actor);
}

void eng::ActorGraph::FlagActorNewParent(Actor* actor) {
	m_NewParentActors.emplace(actor);
}