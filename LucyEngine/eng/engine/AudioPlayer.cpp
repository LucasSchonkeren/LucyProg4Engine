#include "AudioPlayer.h"
#include <cassert>
#include <SDL_mixer.h>
#include <iostream>
#include <map>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

class eng::AudioPlayer::impl {
public:
	impl() {
		//Initialize SDL_mixer
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			assert(false);
		}

		m_Thread = std::jthread{ [&](std::stop_token stopToken) {
			while (!stopToken.stop_requested()) {
				if (!m_SoundsToPlay.empty()) {
					std::unique_lock f_QueueLock{ m_QueueMutex };
					std::string f_Sound{ m_SoundsToPlay.front() };
					m_SoundsToPlay.pop();
					f_QueueLock.unlock();

					std::unique_lock f_SoundLock{ m_SoundMutex };

					if (m_SoundChunks.count(f_Sound) == 0) {
						m_SoundChunks[f_Sound] = Mix_LoadWAV(("../Data/" + f_Sound).c_str());
					}
					Mix_PlayChannel(-1, m_SoundChunks[f_Sound], 0);
				}
				else std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
		};
	};

	~impl() {
		m_Thread.request_stop();

		for (auto& [key, chunk] : m_SoundChunks) {
			if (chunk != nullptr) {
				Mix_FreeChunk(chunk);
				chunk = nullptr;
			}
		}
		m_SoundChunks.clear();

		Mix_CloseAudio();
	}

	void PlaySound(std::string sound) {
		m_SoundsToPlay.push(sound);
	}

	void StopSound(std::string sound) {
		// TODO
	}

	void StopSound() {
		// TODO
	}
private:
	std::map<std::string, Mix_Chunk*> m_SoundChunks{};
	std::queue<std::string> m_SoundsToPlay{};
	std::jthread m_Thread;
	std::mutex m_QueueMutex;
	std::mutex m_SoundMutex;
};

eng::AudioPlayer::AudioPlayer() : m_ImplUptr(std::make_unique<impl>()) {
}

eng::AudioPlayer::~AudioPlayer() = default;

void eng::AudioPlayer::PlaySound(std::string sound) {
	m_ImplUptr->PlaySound(sound);
}

void eng::AudioPlayer::StopSound(std::string sound) {
	m_ImplUptr->StopSound(sound);
}

void eng::AudioPlayer::StopSound() {	
	m_ImplUptr->StopSound();
}					   
					   


eng::LoggingAudioPlayer::LoggingAudioPlayer() {
}

void eng::LoggingAudioPlayer::PlaySound(std::string sound) {
	service::logger.Get().Log("Sound \"" + sound + "\" played");
	m_AudioPlayer.PlaySound(sound);
}

void eng::LoggingAudioPlayer::StopSound(std::string sound) {
	service::logger.Get().Log("Sound \"" + sound + "\" stopped");
	m_AudioPlayer.StopSound(sound);
}

void eng::LoggingAudioPlayer::StopSound() {
	service::logger.Get().Log("All sound stopped");
	m_AudioPlayer.StopSound();
}
