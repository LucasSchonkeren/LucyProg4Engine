#pragma once
#include "../../eng/Actor.h"

namespace eng::scenegraph {

/// <summary>
/// Destroy and move components along the active scenegraph.
/// </summary>
void Cleanup(eng::Actor& sceneRoot);

void SetParentInCleanup(Actor& child, Actor& parent, bool keepWorldTransform);

}