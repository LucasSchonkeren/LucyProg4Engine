#include "Scenegraph.h"
#include "../../eng/Actor.h"

#include <queue>

namespace eng::scenegraph {

//-----------------------------|Cleanup|--------------------------------

// variables

struct ActorMoveInfo {
	Actor& child, & parent;
	bool keepWorldTransform;
};

std::queue<ActorMoveInfo> actorsToMove;

void RemoveDestroyedChildren(eng::Actor& parent) {
	for (auto child : parent.GetChildren()) {
		if (child.get().IsFlagged(eng::Actor::Flags::Destroyed)) {
			parent.DestroyChildActor(&child.get());
		}
		else RemoveDestroyedChildren(child);
	}
}

void Cleanup(eng::Actor& sceneRoot) {

	while (not actorsToMove.empty()) {
		actorsToMove.front().child.SetParent(actorsToMove.front().parent, actorsToMove.front().keepWorldTransform);
		actorsToMove.pop();
	}

	RemoveDestroyedChildren(sceneRoot);
}

void SetParentInCleanup(Actor& child, Actor& parent, bool keepWorldTransform) {
	actorsToMove.emplace(ActorMoveInfo{ child, parent, keepWorldTransform });
}

}