#pragma once

#include "../Actor.h"

namespace eng {

class ICommand {
public:
	virtual ~ICommand() = default;
	virtual bool Execute(Actor& target) = 0;
};

} // !eng

