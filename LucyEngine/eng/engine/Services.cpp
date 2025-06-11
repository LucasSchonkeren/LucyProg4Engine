#include "Services.h"

namespace eng::service {

class NullAudioPlayer final : public IAudioPlayer {
public:
	virtual void PlaySound(std::string) {};
	virtual void StopSound(std::string) {};
	virtual void StopSound() {};
};

Service<IAudioPlayer> audioPlayer{ std::make_unique<NullAudioPlayer>() };

class NullLogger final : public ILogger {
public:
	void Log(std::string text) override {};
};

Service<ILogger> logger{ std::make_unique<NullLogger>() };

class NullResourceLoader final : public IResourceLoader {
public:
	virtual dae::Texture2D* LoadTexture(const std::string&) { return nullptr;  };
	virtual dae::Font* LoadFont(const std::string&, uint8_t) { return nullptr; }
};

Service<IResourceLoader>  resources{ std::make_unique<NullResourceLoader>() };

class NullInput final : public IInput {
public:
	bool ProcessInput() override { return false; };
	void RegisterCommandBinding(u_ptr<input::CommandBindings> binding) override {};
	void UnregisterCommandBinding(u_ptr<input::CommandBindings> binding) override {};
};

Service<IInput> input{ std::make_unique<NullInput>() };

}