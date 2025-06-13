#include "GameTime.h"

double eng::GameTime::DeltaTime() const {
	return m_DeltaTime;
}

void eng::GameTime::UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous) {
	m_DeltaTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - previous).count();
}

int eng::GameTime::MinMilliSecPerFrame() const
{
	return m_MinMillisePerFrame;
}
