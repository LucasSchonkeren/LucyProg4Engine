#pragma once

#include <memory>

#include "../utils/utils.h"
#include "KeyState.h"


namespace eng::input {


class GamepadState final {
public: //--------------------|Constructor, rule of 5|-----------------------
	GamepadState(unsigned int playerNum = 0);
	~GamepadState();

	GamepadState(const GamepadState&) = delete;
	GamepadState& operator=	(const GamepadState&) = delete;

	GamepadState(const GamepadState&&) = delete;
	GamepadState& operator=	(const GamepadState&&) = delete;

public: //----------------|State|-------------------------------------

	void FetchCurrentState();

	bool IsKeyDown(GamepadKeys key);
	bool IsKeyPressed(GamepadKeys key);
	bool IsKeyUp(GamepadKeys key);

private: //---------------|Pimpl|------------------------------------
	class impl;
	u_ptr<impl> m_ImplUptr;
};

} // !eng::input

