#pragma once
#include "../abstract/AbstractCommand.h"
#include "../utils/utils.h"
#include "../Actor.h"
#include "KeyboardState.h"
#include "GamepadState.h"
#include "KeyState.h"

#include <string>
#include <string_view>
#include <ranges>
#include <map>
#include <unordered_map>

namespace eng::input {

//----------------------|CommandBindings|---------------------------------------------

class CommandBindings final {
public:
	//-------------|Constructor, rule of 5|-------------------------------
	
	CommandBindings(Actor* target = nullptr);

	CommandBindings				(const CommandBindings&) = delete;
	CommandBindings& operator=	(const CommandBindings&) = delete;

	CommandBindings				(CommandBindings&&)		 = delete;
	CommandBindings& operator=	(CommandBindings&&)		 = delete;

	//-------------|Command methods|-------------------------------------

	template <std::derived_from<AbstractCommand> commandT, typename... argsT> 
	AbstractCommand* NewCommand(std::string_view commandName, Keystate keystatePtr, argsT... args) {
		m_CommandUptrs.emplace(commandName, std::make_unique<commandT>(args...));
		BindCommand(keystatePtr, commandName);
		return m_CommandUptrs[commandName].get();
	};

	void				RemoveCommand(std::string_view commandName);

	//--------------|Keybind methods|-----------------------------------

	void				BindCommand(Keystate, std::string_view commandName);
	void				UnBindCommand(std::string_view commandName);
	void				UnbindKey(Keystate keystate);

	const std::map<Keystate, AbstractCommand*>& GetCommandBindings();

	//------------------|Target methods|------------------------------

	void				SetTargetActor(Actor* target);
	Actor*				GetTargetActor();
private:
	Actor* m_Target{};
	std::unordered_map<std::string_view, u_ptr<AbstractCommand>> m_CommandUptrs{};
	std::map<Keystate, AbstractCommand*> m_CommandBindings{};
};

} // !eng::input


