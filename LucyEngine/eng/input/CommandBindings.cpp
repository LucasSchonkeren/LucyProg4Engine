#include "CommandBindings.h"
#include "../abstract/AbstractCommand.h"
#include "../utils/utils.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <map>
#include <unordered_map>
#include <Windows.h>
#include <string>
#include <algorithm>

namespace eng::input {
//--------------------------|CommandBindings|------------------------------------------------------

CommandBindings::CommandBindings(Actor* target) :
	m_Target(target) {

}


void CommandBindings::RemoveCommand(std::string_view commandName) {
	UnBindCommand(commandName);
	m_CommandUptrs.erase(commandName);
}

void CommandBindings::BindCommand(Keystate keystate, std::string_view commandName) {
	m_CommandBindings[keystate] = m_CommandUptrs[commandName].get();
}

void CommandBindings::UnBindCommand(std::string_view commandName) {
	std::erase_if(m_CommandBindings, [&](const auto& pair) {
		return pair.second == m_CommandUptrs[commandName].get();
		});
}

void CommandBindings::UnbindKey(Keystate keystate) {
	std::erase_if(m_CommandBindings, [&](const auto& pair) {
		return pair.first == keystate;
		});
}

const std::map<Keystate, ICommand*>& CommandBindings::GetCommandBindings() {
	return m_CommandBindings;
}

void CommandBindings::SetTargetActor(Actor* target) {
	m_Target = target;
}

Actor* CommandBindings::GetTargetActor() {
	return m_Target;
}

} // !eng::input