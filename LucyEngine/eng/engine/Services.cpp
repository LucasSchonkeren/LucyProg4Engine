#include "Services.h"

namespace eng::service {

class NullAudioPlayer final : public IAudioPlayer {
public:
	virtual void PlaySound(std::string sound) {};
	virtual void StopSound(std::string sound) {};
	virtual void StopSound() {};
};

Service<IAudioPlayer> audioPlayer{ std::make_unique<NullAudioPlayer>() };

class NullLogger final : public ILogger {
public:
	void Log(std::string text) override {};
};

Service<ILogger> logger{ std::make_unique<NullLogger>() };

}