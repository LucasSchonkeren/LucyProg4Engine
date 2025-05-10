#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"

#include <glm.hpp>

class eng::Actor;

namespace eng::cpt {

class GridTransform final : public eng::AbstractComponent, public IObserver {
public: //---------------|Constructor/Destructor/copy/move|--------------

	GridTransform(eng::Actor& owner) : AbstractComponent(owner) {};
	~GridTransform() = default;

	GridTransform(const GridTransform&) = delete;
	GridTransform& operator=	(const GridTransform&) = delete;

	GridTransform(GridTransform&&) = delete;
	GridTransform& operator=	(GridTransform&&) = delete;

public: //------------------|Transform Methods|--------------------------

	/// <summary>
	/// Allign the actor's transform with its grid position
	/// </summary>
	void AllignTransform();

public: //------------------|Gameloop Methods|--------------------------
	
	void Start() override;
	void Update() override;

public: //------------------|Observer|--------------------------

	virtual void OnEvent(Event event);


/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Grid fields|----------------------------
	
	glm::ivec2 m_Position;

}; // !FpsTracker

} // !cpt