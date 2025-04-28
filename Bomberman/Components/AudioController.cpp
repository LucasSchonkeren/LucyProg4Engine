#include "AudioController.h"
#include "eng/engine/Services.h"

void eng::cpt::AudioController::OnEvent(Event event) {
	service::IAudioPlayer& f_AudioPlayer{ service::audioPlayer.Get() };

	switch (event.eventTypeHash) {
	case 0:
		break;
	default:
		f_AudioPlayer.PlaySound("score.wav");
	}
}
