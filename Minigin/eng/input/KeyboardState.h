#pragma once

#include "../utils/Utils.h"

#include <bitset>

namespace eng::input {



class KeyboardState final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	KeyboardState()		= default;
	~KeyboardState()	= default;

	KeyboardState				(const KeyboardState&)	= delete;
	KeyboardState& operator=	(const KeyboardState&)	= delete;

	KeyboardState				(const KeyboardState&&)	= delete;
	KeyboardState& operator=	(const KeyboardState&&)	= delete;

public: //--------------|State|-------------------

	void FetchCurrentState();

	bool IsKeyDown(KeyboardKeys key);
	bool IsKeyPressed(KeyboardKeys key);
	bool IsKeyUp(KeyboardKeys key);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Button state|----------------------------

	std::bitset<static_cast<int>(KeyboardKeys::SIZE)> m_DownKeys{};
	std::bitset<static_cast<int>(KeyboardKeys::SIZE)> m_PressedKeys{};
	std::bitset<static_cast<int>(KeyboardKeys::SIZE)> m_UpKeys{};

}; // !KeyboardState

} // !eng::input