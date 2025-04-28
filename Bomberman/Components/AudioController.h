#pragma once
#include "eng/abstract/AbstractComponent.h"
#include "eng/abstract/AbstractObserver.h"

namespace eng::cpt {

class AudioController final : public eng::AbstractComponent, public IObserver  {
public: //---------------|Constructor/Destructor/copy/move|--------------

	AudioController(eng::Actor& owner) : AbstractComponent(owner) {};
	~AudioController() = default;

	AudioController(const AudioController&) = delete;
	AudioController& operator=	(const AudioController&) = delete;

	AudioController(const AudioController&&) = delete;
	AudioController& operator=	(const AudioController&&) = delete;

public: //--------------------|Observer methods|---------------------------

	virtual void OnEvent(Event event);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

}; // !Rotator

} // !cpt