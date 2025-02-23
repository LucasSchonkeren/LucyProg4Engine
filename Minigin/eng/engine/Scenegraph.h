#pragma once
#include "../../eng/Actor.h"

namespace eng::scenegraph {

/// <summary>
/// Destroy and move components along the active scenegraph.
/// </summary>
void Cleanup(eng::Actor& sceneRoot);

void ChangeParent(Actor& child, Actor& parent);

}