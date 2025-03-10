#include "GamepadState.h"

#include <windows.h>
#include <Xinput.h>
#include <bitset>
#include <iostream>
#include <format>
#include <cassert>

namespace eng::input {

DWORD GamepadKeyToXinputDword(GamepadKeys key) {
	switch (key)
	{
	case eng::input::GamepadKeys::Up:
		return XINPUT_GAMEPAD_DPAD_UP;
	case eng::input::GamepadKeys::Down:
		return XINPUT_GAMEPAD_DPAD_DOWN;
	case eng::input::GamepadKeys::Left:
		return XINPUT_GAMEPAD_DPAD_LEFT;
	case eng::input::GamepadKeys::Right:
		return XINPUT_GAMEPAD_DPAD_RIGHT;
	case eng::input::GamepadKeys::Start:
		return XINPUT_GAMEPAD_START;
	case eng::input::GamepadKeys::Back:
		return XINPUT_GAMEPAD_BACK;
	case eng::input::GamepadKeys::Lthumb:
		return XINPUT_GAMEPAD_LEFT_THUMB;
	case eng::input::GamepadKeys::Rthumb:
		return XINPUT_GAMEPAD_RIGHT_THUMB;
	case eng::input::GamepadKeys::Lshoulder:
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case eng::input::GamepadKeys::Rshoulder:
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case eng::input::GamepadKeys::A:
		return XINPUT_GAMEPAD_A;
	case eng::input::GamepadKeys::B:
		return XINPUT_GAMEPAD_B;
	case eng::input::GamepadKeys::X:
		return XINPUT_GAMEPAD_X;
	case eng::input::GamepadKeys::Y:
		return XINPUT_GAMEPAD_Y;
	default:
		assert(false and "Invalid gamepad key");
		return 0;
	}
}

class GamepadState::impl final {
public:
	GamepadState::impl(unsigned int playerNum) :
		m_PlayerNum(playerNum) {

	}

public:
	void FetchCurrentState() {
		XINPUT_STATE f_InputState;
		SecureZeroMemory(&f_InputState, sizeof(XINPUT_STATE));
		if (XInputGetState(m_PlayerNum, &f_InputState) == ERROR_DEVICE_NOT_CONNECTED) return; // Controller not attached

		if (f_InputState.dwPacketNumber == m_LastPacketNumber) return; // Data hasn't changes
		m_LastPacketNumber = f_InputState.dwPacketNumber; // Update packet number

		auto f_PrevPressedKeys{ m_PressedKeys };
		m_PressedKeys = f_InputState.Gamepad.wButtons;
		m_DownKeys = ~f_PrevPressedKeys & m_PressedKeys;
		m_UpKeys = f_PrevPressedKeys & ~m_PressedKeys;
	}

	bool IsKeyDown(GamepadKeys key) {
		return m_DownKeys & GamepadKeyToXinputDword(key);
	}
	bool IsKeyPressed(GamepadKeys key) {
		return m_PressedKeys & GamepadKeyToXinputDword(key);

	}
	bool IsKeyUp(GamepadKeys key) {
		return m_UpKeys & GamepadKeyToXinputDword(key);

	}
private: //---------------|metadata|------------

	const unsigned int m_PlayerNum;
	DWORD m_LastPacketNumber{};

private: //------------|State|---------------
	DWORD m_PressedKeys{};
	DWORD m_UpKeys{};
	DWORD m_DownKeys{};
};

GamepadState::GamepadState(unsigned int playerNum) :
	m_ImplUptr(std::make_unique<impl>(playerNum)) {

}

GamepadState::~GamepadState() = default;

void GamepadState::FetchCurrentState() {
	m_ImplUptr->FetchCurrentState();
}

bool GamepadState::IsKeyDown(GamepadKeys key) {
	return m_ImplUptr->IsKeyDown(key);
}

bool GamepadState::IsKeyPressed(GamepadKeys key) {
	return m_ImplUptr->IsKeyPressed(key);

}

bool eng::input::GamepadState::IsKeyUp(GamepadKeys key)
{
	return m_ImplUptr->IsKeyUp(key);
}

} // !eng::input