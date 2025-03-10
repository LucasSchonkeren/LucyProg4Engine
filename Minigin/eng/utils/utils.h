#pragma once

#include <memory>
#include <vector>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <ranges>

//------------------|Defines|-----------------

#define val_get() value().get()


//-------------|Typedefs|------------------

// Unique Pointer aliases because typenames get looooong

template<typename T>
using u_ptr = std::unique_ptr<T>;

template <typename T>
using u_ptr_vec = std::vector<std::unique_ptr<T>>;

template <typename T>
using u_ptr_set = std::set<std::unique_ptr<T>>;

template <typename T>
using ref_vec = std::vector<std::reference_wrapper<T>>;

template <typename T>
using type_map_unordered = std::unordered_map<std::type_index, T>;

template <typename T>
using u_ptr_type_map_unordered = std::unordered_map<std::type_index, std::unique_ptr<T>>;

//--------------------|Concepts|-----------------------

template <typename R, typename V>
concept RangeOf = std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, V>;


//--------------------------|Input utils|------------------

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
		isKeyboardKey(true), keyboardKey(key), phase(keyPhase){
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

}
