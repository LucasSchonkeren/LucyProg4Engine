#pragma once

#include "../utils/utils.h"
#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"

namespace eng::input {

bool ProcessInput();

void RegisterCommandBinding(u_ptr<CommandBindings> binding);

void UnregisterCommandBinding(u_ptr<CommandBindings> binding);

} // !eng::input

