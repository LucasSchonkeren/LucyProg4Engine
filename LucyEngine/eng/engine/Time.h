#pragma once
#include <chrono>

namespace eng::time {

//----------------------|Consts|-----------------------------

constexpr int desiredFps{100};

constexpr int MinNanoSecPerFrame() { return 1'000'000'000 / desiredFps; }

//----------------------|Delta time|-----------------------------

double	DeltaTime();

void	UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous);

//---------------------|Gameloop|-----------------------------
enum class Stages {
	None, 
	Start,
	Input,
	Update,
	Render,
	Cleanup

};

extern Stages stage;

} // !eng::meta