#pragma once

#include <iostream>

#include "Services.h"

namespace eng {

class ConsoleLogger final : public service::ILogger {
public:
	void Log(std::string text) override {
		std::cout << "[" <<__TIMESTAMP__ << "] "<< text << "\n";
	}
};

} // !eng