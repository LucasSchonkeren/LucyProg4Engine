#pragma once
#include <string>
#include <memory>

namespace eng::service {
//----------------------------------------|Service class|---------------------------------------
template <typename T>
class Service final {
public:
	Service(std::unique_ptr<T> defaultServiceUptr) : m_DefaultServiceUptr(std::move(defaultServiceUptr)), m_ServicePtr(m_DefaultServiceUptr.get()) {};

	void Subscribe(T& service) {
		m_ServicePtr = &service;
	};
	
	void Unsubscribe() {
		m_ServicePtr = m_DefaultServiceUptr;
	};

	T& Get() {
		return *m_ServicePtr;
	};
private:
	T* m_ServicePtr; // -> Unique ptr?
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

}