#pragma once

#include "../utils/utils.h"

namespace eng::input {

enum class KeyPhase {
	Down,
	Pressed,
	Up
};

ENUM_TOSTRING_DECL(KeyPhase)

enum class KeyboardKeys {
	W,
	A,
	S,
	D,
	Space,
	Q,
	E,

	SIZE
};

ENUM_TOSTRING_DECL(KeyboardKeys)

enum class GamepadKeys {
	Up,
	Down,
	Left,
	Right,
	Start,
	Back,
	Lthumb,
	Rthumb,
	Lshoulder,
	Rshoulder,
	A,
	B,
	X,
	Y,

	SIZE
};

ENUM_TOSTRING_DECL(GamepadKeys)

struct Keystate {
	Keystate(KeyboardKeys key, KeyPhase keyPhase) :
		isKeyboardKey(true), keyboardKey(key), phase(keyPhase) {
	}
	Keystate(GamepadKeys key, KeyPhase keyPhase) :
		isKeyboardKey(false), gamepadKey(key), phase(keyPhase) {
	}

	const bool isKeyboardKey;
	union {
		KeyboardKeys keyboardKey;
		GamepadKeys gamepadKey;
	};
	KeyPhase phase;

	// Define the <=> operator for Keystate. This is necessary to create a total ordering between keystates (for use in ordered containers)
	auto operator<=>(const Keystate& other) const {
		if (auto cmp = isKeyboardKey <=> other.isKeyboardKey; cmp != 0) return cmp;

		if (isKeyboardKey) {
			if (auto cmp = keyboardKey <=> other.keyboardKey; cmp != 0) return cmp;
		}
		else {
			if (auto cmp = gamepadKey <=> other.gamepadKey; cmp != 0) return cmp;
		}

		return phase <=> other.phase;
	}

	// Define the == operator for Keystate
	bool operator==(const Keystate& other) const {
		if (isKeyboardKey != other.isKeyboardKey) return false;
		if (isKeyboardKey) {
			return keyboardKey == other.keyboardKey && phase == other.phase;
		}
		else {
			return gamepadKey == other.gamepadKey && phase == other.phase;
		}
	}
};

} // !eng::input