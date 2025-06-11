#include "Rotator.h"
#include "eng/Actor.h"

void cpt::Rotator::Update() {
	m_TotalTime += Owner().DeltaTime() * m_Speed;

	Owner().GetTransform().SetLocalPosition(static_cast<float>(cos(m_TotalTime)) * m_Radius, static_cast<float>(sin(m_TotalTime)) * m_Radius);
}