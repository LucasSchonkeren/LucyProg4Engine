#pragma once

#include <unordered_map>
#include <tuple>

#include "glm.hpp"
#include "../Actor.h"
#include "../abstract/AbstractObserver.h"
#include "../utils/utils.h"

namespace eng::cpt {

struct GridTileData {
	bool passable;
};

class Grid final : public AbstractComponent, public IObserver {
public: //---------------|Static helper functions|--------------


	/// <returns>The first Grid Component found upwards the actor tree</returns>
	static Grid* FindParentGrid(Actor& actor);

public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Grid(eng::Actor& owner, float pixelWidth) : AbstractComponent(owner), m_PixelWidth( pixelWidth ) {};
	~Grid()	= default;

	Grid				(const Grid&)	= delete;
	Grid& operator=		(const Grid&)	= delete;

	Grid				(const Grid&&)	= delete;
	Grid& operator=		(const Grid&&)	= delete;

public: //---------------|Grid Methods|---------------------------

	float GetPixelWidth() const;
	glm::vec2 GridToWorldPosition(glm::ivec2 gridPosition);

public: //---------------|Observer|---------------------------

	void OnEvent(Event event) override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	float m_PixelWidth;

	std::unordered_map<std::pair<int,int>, GridTileData, hash_pair> m_TileData;

}; // !Grid

} // !_namespace_