#pragma once
#include "Services.h"

namespace eng {

class AudioPlayer final : public service::IAudioPlayer {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	AudioPlayer();
	~AudioPlayer();

	AudioPlayer				(const AudioPlayer&)	= delete;
	AudioPlayer& operator=	(const AudioPlayer&)	= delete;

	AudioPlayer				(const AudioPlayer&&)	= delete;
	AudioPlayer& operator=	(const AudioPlayer&&)	= delete;

public: //---------------|Audio method implementations|--------------

    void PlaySound(std::string sound)	override;
	void StopSound(std::string sound)	override;
	void StopSound()					override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Pimpl|----------------------------
	class impl;
	std::unique_ptr<impl> m_ImplUptr;
}; // !AudioPlayer

class LoggingAudioPlayer final : public service::IAudioPlayer {
public: //---------------|Constructor/Destructor/copy/move|--------------

	LoggingAudioPlayer();
	~LoggingAudioPlayer() = default;

	LoggingAudioPlayer(const LoggingAudioPlayer&) = delete;
	LoggingAudioPlayer& operator=	(const LoggingAudioPlayer&) = delete;

	LoggingAudioPlayer(const LoggingAudioPlayer&&) = delete;
	LoggingAudioPlayer& operator=	(const LoggingAudioPlayer&&) = delete;

public: //---------------|Audio method implementations|--------------

	void PlaySound(std::string sound)	override;
	void StopSound(std::string sound)	override;
	void StopSound()					override;

/*##################################|PRIVATE|##################################################*/

private: //---------------|AudioPlayer|--------------

	AudioPlayer m_AudioPlayer{};

}; // !LoggingAudioPlayer

} // !eng