#include "Rotator.h"
#include "eng/engine/Time.h"
#include "eng/Actor.h"

void cpt::Rotator::Update() {
	m_TotalTime += eng::time::DeltaTime() * m_Speed;

	GetOwner().GetTransform().SetLocalPosition(static_cast<float>(cos(m_TotalTime)) * m_Radius, static_cast<float>(sin(m_TotalTime)) * m_Radius);
}