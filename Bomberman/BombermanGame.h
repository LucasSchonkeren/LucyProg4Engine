#pragma once

#include "eng/Game/AbstractGame.h"
#include "eng/Actor.h"

namespace bmbMan {

class BombermanGame final : public eng::AbstractGame {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	BombermanGame();
	~BombermanGame() override = default;

	BombermanGame				(const BombermanGame&)	= delete;
	BombermanGame& operator=	(const BombermanGame&)	= delete;

	BombermanGame				(const BombermanGame&&)	= delete;
	BombermanGame& operator=	(const BombermanGame&&)	= delete;

public:

	eng::Actor& RootActor() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	u_ptr<eng::Actor> m_Root;

}; // !BombermanGame

} // !bmbMan