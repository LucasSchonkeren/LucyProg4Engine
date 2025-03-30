#include "Input.h"
#include "SDL.h"
#include "backends/imgui_impl_sdl2.h"

#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"

#include <iostream>

namespace eng::input {

KeyboardState keyboardState{};
GamepadState gamePadState{};

std::set<u_ptr<CommandBindings>> commandBindingUptrs{};

bool ProcessInput() {
	// Poll SDL evdents
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}

		//ImGui_ImplSDL2_ProcessEvent(&e);
	}

	keyboardState.FetchCurrentState();
	gamePadState.FetchCurrentState();

	for (auto& bindingPtr : commandBindingUptrs) {
		if (not bindingPtr->GetTargetActor()) break;
		for (const auto& cmdBinding : bindingPtr->GetCommandBindings()) {
			if (cmdBinding.first.isKeyboardKey)
				switch (cmdBinding.first.phase) {
				case KeyPhase::Down:
					if (keyboardState.IsKeyDown(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Pressed:
					if (keyboardState.IsKeyPressed(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Up:
					if (keyboardState.IsKeyUp(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				}
			else {
				switch (cmdBinding.first.phase) {
				case KeyPhase::Down:
					if (gamePadState.IsKeyDown(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Pressed:
					if (gamePadState.IsKeyPressed(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Up:
					if (gamePadState.IsKeyUp(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				}
			}
		}
	}

	return true;
}

void RegisterCommandBinding(u_ptr<CommandBindings> binding) {
	commandBindingUptrs.emplace(std::move(binding));
}

void UnregisterCommandBinding(u_ptr<CommandBindings> binding) {
	commandBindingUptrs.erase(std::move(binding));
}

}