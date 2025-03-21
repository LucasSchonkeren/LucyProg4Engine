#ifndef NOSDL
#include <SDL.h>
#endif

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "../dae/Minigin.h"
#include "../dae/ResourceManager.h"

#include "utils/utils.h"


#ifndef NOSDL
int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run();

	return 0;
}
#endif // !ifndef NOSDL

#ifdef NOSDL

#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../libs/catch.hpp"

TEST_CASE("The Catch2 framework works") {
	REQUIRE(true);
}

#endif
