#pragma once

#include "../Actor.h"

namespace eng {

class AbstractCommand {
public:
	virtual ~AbstractCommand() = default;
	virtual bool Execute(Actor& target) = 0;
};

} // !eng