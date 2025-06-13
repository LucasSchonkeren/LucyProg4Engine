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

class NullRenderer final : public IRenderer {
public:
	virtual void Render(eng::Actor&) const {}

	virtual void RenderTexture(const dae::Texture2D &, const SDL_Rect &, const SDL_Rect &) const {}

	virtual SDL_Renderer* GetSDLRenderer() const { return nullptr; }

	virtual const SDL_Color& GetBackgroundColor() const { return m_Color; }
	virtual void SetBackgroundColor(const SDL_Color&) {}
private:
	SDL_Color m_Color{};
};

Service<IRenderer> renderer{ std::make_unique<NullRenderer>() };


}