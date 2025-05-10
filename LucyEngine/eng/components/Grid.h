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
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Grid(eng::Actor& owner) : AbstractComponent(owner) {};
	~Grid()	= default;

	Grid				(const Grid&)	= delete;
	Grid& operator=		(const Grid&)	= delete;

	Grid				(const Grid&&)	= delete;
	Grid& operator=		(const Grid&&)	= delete;

public: //---------------|Observer|---------------------------

	void OnEvent(Event event) override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	float m_GridPixelWidth{ 20 };

	std::unordered_map<std::pair<int,int>, GridTileData, hash_pair> m_TileData;

}; // !Grid

} // !_namespace_