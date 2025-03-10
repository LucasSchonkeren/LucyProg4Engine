#include "InputProcessor.h"
#include "SDL.h"
#include "backends/imgui_impl_sdl2.h"

#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"

#include <iostream>

namespace eng::input {

//---------------------|Interface|----------------------------

bool InputProcessor::m_Instantiated{ false };

bool InputProcessor::operator()() {
	// Poll SDL evdents
	SDL_Event e;
	while (SDL_PollEvent(&e))  {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	m_KeyboardState.FetchCurrentState();
	m_GamepadState.FetchCurrentState();

	for (auto* bindingPtr : m_CommandBindings) {
		if (not bindingPtr->GetTargetActor()) break;
		for (const auto& cmdBinding : bindingPtr->GetCommandBindings()) {
			if (cmdBinding.first.isKeyboardKey)
				switch (cmdBinding.first.phase) {
				case KeyPhase::Down:
					if (m_KeyboardState.IsKeyDown(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Pressed:
					if (m_KeyboardState.IsKeyPressed(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Up:
					if (m_KeyboardState.IsKeyUp(cmdBinding.first.keyboardKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				}
			else {
				switch (cmdBinding.first.phase) {
				case KeyPhase::Down:
					if (m_GamepadState.IsKeyDown(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Pressed:
					if (m_GamepadState.IsKeyPressed(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				case KeyPhase::Up:
					if (m_GamepadState.IsKeyUp(cmdBinding.first.gamepadKey)) cmdBinding.second->Execute(*bindingPtr->GetTargetActor());
					break;
				}
			}
		}
	}

	return true;
}

InputProcessor::InputProcessor(std::vector<CommandBindings*> bindings) :
	m_CommandBindings(bindings) {
	assert(!m_Instantiated and "Only one Input Processor may be instantiated"); \
		m_Instantiated = true;
}

InputProcessor::~InputProcessor() = default;

}