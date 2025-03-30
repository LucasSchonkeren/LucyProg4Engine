#pragma once

namespace eng::input {

enum class KeyPhase {
	Down,
	Pressed,
	Up
};

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

struct Keystate {
	Keystate(KeyboardKeys key, KeyPhase keyPhase) :
		isKeyboardKey(true), keyboardKey(key), phase(keyPhase) {
	}
	Keystate(GamepadKeys key, KeyPhase keyPhase) :
		isKeyboardKey(false), gamepadKey(key), phase(keyPhase) {
	}

	bool isKeyboardKey;
	union {
		KeyboardKeys keyboardKey;
		GamepadKeys gamepadKey;
	};
	KeyPhase phase;

	// Define the <=> operator for Keystate
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