#pragma once
#include <string>
#include <memory>

#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"
#include "../utils/utils.h"
#include "../input/CommandBindings.h"
#include "../input/KeyboardState.h"
#include "../input/GamepadState.h"

namespace eng::service {
//----------------------------------------|Service class|---------------------------------------
template <typename T>
class Service final {
public:
	Service(std::unique_ptr<T> defaultServiceUptr) : m_DefaultServiceUptr(std::move(defaultServiceUptr)), m_ServicePtr(m_DefaultServiceUptr.get()) {};

	void Register(std::unique_ptr<T> service) {
		m_ServicePtr = std::move(service);
	};
	
	void Unregister() {
		m_ServicePtr = m_DefaultServiceUptr;
	};

	T& Get() {
		return *m_ServicePtr;
	};
private:
	std::unique_ptr<T> m_ServicePtr;
	std::unique_ptr<T> m_DefaultServiceUptr;
};

//----------------------------------------|Services|---------------------------------------

class IAudioPlayer {
public:
	virtual ~IAudioPlayer() = default;
	virtual void PlaySound(std::string sound) = 0;
	virtual void StopSound(std::string sound) = 0;
	virtual void StopSound() = 0;
};

extern Service<IAudioPlayer> audioPlayer;

class ILogger {
public:
	virtual ~ILogger() = default;
	virtual void Log(std::string text) = 0;
};

extern Service<ILogger> logger;

class IResourceLoader {
public:
	virtual ~IResourceLoader() = default;

	virtual dae::Texture2D* LoadTexture(const std::string& file) = 0;
	virtual dae::Font* LoadFont(const std::string& file, uint8_t size) = 0;
};

extern Service<IResourceLoader>  resources;

class IInput {
public:
	virtual ~IInput() = default;

	virtual bool ProcessInput() = 0;
	virtual void RegisterCommandBinding(u_ptr<input::CommandBindings> binding) = 0;
	virtual void UnregisterCommandBinding(u_ptr<input::CommandBindings> binding) = 0;
};

extern Service<IInput> input;

}