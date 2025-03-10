#include "KeyboardState.h"
#include "SDL.h"
#include <cassert>

namespace eng::input {

SDL_Scancode KeyboardKeyToSdlScancode(KeyboardKeys key) {
	switch (key)
	{
	case eng::input::KeyboardKeys::W:
		return SDL_SCANCODE_W;
	case eng::input::KeyboardKeys::A:
		return SDL_SCANCODE_A;
	case eng::input::KeyboardKeys::S:
		return SDL_SCANCODE_S;
	case eng::input::KeyboardKeys::D:
		return SDL_SCANCODE_D;
	case eng::input::KeyboardKeys::Space:
		return SDL_SCANCODE_SPACE;
	default:
		assert(false and "Key not implemented by engine");
		return SDL_SCANCODE_0;
	}
}

void KeyboardState::FetchCurrentState() {
	int f_SdlKeyboardSize;
	const Uint8* f_SdlKeyboardState{ SDL_GetKeyboardState(&f_SdlKeyboardSize) };

	// Save previous pressed key state
	auto f_PrevPressedKeys{ m_PressedKeys };

	// Set current pressed key state
	for (int i{}; i < static_cast<int>(KeyboardKeys::SIZE); ++i) {
		SDL_Scancode f_SdlScancode{ KeyboardKeyToSdlScancode(static_cast<KeyboardKeys>(i)) };
		if (f_SdlKeyboardState[f_SdlScancode] and not m_PressedKeys.test(i)) m_PressedKeys.set(i);
		else if (not f_SdlKeyboardState[f_SdlScancode] and m_PressedKeys.test(i)) m_PressedKeys.reset(i);
	}

	//Calculate downed/released keys this frame

	m_DownKeys = ~f_PrevPressedKeys & m_PressedKeys;
	m_UpKeys = f_PrevPressedKeys & ~m_PressedKeys;
}

bool KeyboardState::IsKeyDown(KeyboardKeys key) {
	return m_DownKeys.test(static_cast<int>(key));
}

bool KeyboardState::IsKeyUp(KeyboardKeys key) {
	return m_UpKeys.test(static_cast<int>(key));
}

bool KeyboardState::IsKeyPressed(KeyboardKeys key) {
	return m_PressedKeys.test(static_cast<int>(key));
}

} // !eng::input