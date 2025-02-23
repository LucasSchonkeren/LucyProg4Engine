#include "Scenegraph.h"
#include "../../eng/Actor.h"

#include <queue>

namespace eng::scenegraph {

//-----------------------------|Cleanup|--------------------------------

// variables

std::queue<std::pair<Actor*, Actor*>> actorsToMove;

void RemoveDestroyedChildren(eng::Actor& parent) {
	for (auto child : parent.GetChildren()) {
		if (child.get().IsFlagged(eng::Actor::Flags::Destroyed)) {
			parent.RemoveChildActor(&child.get());
		}
		else RemoveDestroyedChildren(child);
	}
}

void Cleanup(eng::Actor& sceneRoot) {

	while (not actorsToMove.empty()) {
		actorsToMove.front().first->SetParent(*actorsToMove.front().second);
		actorsToMove.pop();
	}

	RemoveDestroyedChildren(sceneRoot);
}

//--------------------------|Scenegraph control|----------------------------

void ChangeParent(Actor& child, Actor& parent) {
	actorsToMove.emplace(std::make_pair<Actor*, Actor*>(&child, &parent));
}

}