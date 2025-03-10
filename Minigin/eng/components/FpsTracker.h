#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"

class eng::Actor; 

namespace cpt {

class FpsTracker final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	FpsTracker(eng::Actor& owner) : AbstractComponent(owner) {};
	~FpsTracker() = default;

	FpsTracker(const FpsTracker&) = default;
	FpsTracker& operator=	(const FpsTracker&) = default;

	FpsTracker(FpsTracker&&) = default;
	FpsTracker& operator=	(FpsTracker&&) = default;

public: //------------------|Gameloop Methods|--------------------------
	void Start() override;
	void Update() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Component references|----------------------------

	TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker

} // !cpt