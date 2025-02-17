#pragma once

#include "../AbstractComponent.h"

namespace cpt {

class FpsTracker final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	FpsTracker()	= default;
	~FpsTracker()	= default;

	FpsTracker				(const FpsTracker&)	= default;
	FpsTracker& operator=	(const FpsTracker&)	= default;

	FpsTracker				(FpsTracker&&)	= default;
	FpsTracker& operator=	(FpsTracker&&)	= default;

public: //------------------|Gameloop Methods|--------------------------
	void Update() override;

/*##################################|PRIVATE|##################################################*/



private: //---------------------------|Fields|----------------------------

}; // !FpsTracker

} // !cpt