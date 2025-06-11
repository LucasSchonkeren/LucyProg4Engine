#pragma once

#include "../utils/utils.h"
#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"
#include "../engine/Services.h"

namespace eng::input {

class Input final : public eng::service::IInput {
	bool ProcessInput();
	void RegisterCommandBinding(u_ptr<CommandBindings> binding);
	void UnregisterCommandBinding(u_ptr<CommandBindings> binding);
};

} // !eng::input

