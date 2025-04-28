#pragma once
#include "eng/abstract/AbstractComponent.h"

namespace cpt {

class Rotator final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Rotator(eng::Actor& owner, float radius, float speed) : AbstractComponent(owner), m_Radius(radius), m_Speed(speed) {};
	~Rotator()	= default;

	Rotator				(const Rotator&)	= delete;
	Rotator& operator=	(const Rotator&)	= delete;

	Rotator				(const Rotator&&)	= delete;
	Rotator& operator=	(const Rotator&&)	= delete;

public: //--------------------|Gameloop methods|---------------------------

	void Update() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	float m_Radius{5};
	float m_Speed{ 1 };
	double m_TotalTime{};

}; // !Rotator

} // !cpt