#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"

class eng::Actor; 

namespace eng::cpt {

class FpsTracker final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	FpsTracker(eng::Actor& owner) : AbstractComponent(owner) {};
	~FpsTracker() = default;

	FpsTracker(const FpsTracker&) = delete;
	FpsTracker& operator=	(const FpsTracker&) = delete;

	FpsTracker(FpsTracker&&) = delete;
	FpsTracker& operator=	(FpsTracker&&) = delete;

public: //------------------|Gameloop Methods|--------------------------
	void Start() override;
	void Update() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Component references|----------------------------

	TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker

} // !cpt