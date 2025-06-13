#pragma once

#include <chrono>

namespace eng {

class IGameTime {
public:
	virtual ~IGameTime() = default;

	virtual double	DeltaTime() const = 0;
	virtual void	UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous) = 0;
	virtual int		MinMilliSecPerFrame() const = 0;
};

class GameTime final : public IGameTime {
public: //---------------------|Time methods|-----------------------------------

	double	DeltaTime() const override;
	void	UpdateDeltaTime(std::chrono::time_point<std::chrono::steady_clock> const& previous) override;
	int		MinMilliSecPerFrame() const override;

private: //---------------------------|time fields|-----------------------------

	double m_DeltaTime{};
	int m_MinMillisePerFrame{1000/100};
};

}