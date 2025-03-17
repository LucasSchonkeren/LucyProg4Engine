#pragma once

#include "../utils/utils.h"
#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"

namespace eng::input {

bool ProcessInput();

void RegisterCommandBinding(CommandBindings* binding);

void UnregisterCommandBinding(CommandBindings* binding);

} // !eng::input

