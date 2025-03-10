#pragma once

#include "../utils/utils.h"
#include "CommandBindings.h"
#include "KeyboardState.h"
#include "GamepadState.h"

namespace eng::input {

class InputProcessor final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	InputProcessor(std::vector<CommandBindings*> commandBindings);
	~InputProcessor();

	InputProcessor				(const InputProcessor&)	= delete;
	InputProcessor& operator=	(const InputProcessor&)	= delete;

	InputProcessor				(const InputProcessor&&)	= delete;
	InputProcessor& operator=	(const InputProcessor&&)	= delete;


	bool operator() ();

/*##################################|PRIVATE|##################################################*/

private: //-------------------|Single instance enforcement|---------------
	static bool m_Instantiated;

private: //-------------------|Command Bindings|-------------------
	std::vector<CommandBindings*> m_CommandBindings;

private: //--------------------|Key states|---------------------
	KeyboardState m_KeyboardState{};
	GamepadState m_GamepadState{};

}; // !InputProcessor

} // !eng::input

