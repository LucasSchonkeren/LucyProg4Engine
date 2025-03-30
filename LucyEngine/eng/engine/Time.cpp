#include "Time.h"

namespace eng::time {

//-------------|Namespace variables|------------------

double deltaTime{};
Stages stage{};

//----------------|Namespace functions|----------------

double DeltaTime() {
	return deltaTime;
}

void eng::time::UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous) {
	deltaTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - previous).count();
}

} // !eng::time