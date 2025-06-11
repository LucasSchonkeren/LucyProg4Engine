#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"
#include "Grid.h"

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

public: //------------------|Allignment Methods|--------------------------

	/// <returns>The x and y 'allignment' of the owning actor's transform with its parent grid. Allignment is a decimal number between -0.5 and 0.5, representing a percentual range of how far removed the transform is from the center of its grid square, i.e. the 'local' position within the the square</returns>
	glm::vec2 GetAllignment();
	/// <summary>
	/// Allign the actor's transform with its grid position, setting allignment to (0,0)
	/// </summary>
	void AllignWithGrid();

public: //------------------|Gameloop Methods|--------------------------
	
	void OnEnable() override;
	void Start() override;
	void Update() override;
	void OnDisable() override;

public: //------------------|Observer|--------------------------

	virtual void OnEvent(Event event);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Private Grid methods|----------------------------

	void UpdatePosition();

private: //---------------------------|Dirty flags|----------------------------

	bool m_DirtyPosition{};

private: //---------------------------|Grid fields|----------------------------
	
	Grid* m_GridPtr{};

	glm::ivec2 m_Position{};

}; // !FpsTracker

} // !cpt