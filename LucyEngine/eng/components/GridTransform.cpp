#include "GridTransform.h"
#include "../Actor.h"

void eng::cpt::GridTransform::AllignTransform() {

}

void eng::cpt::GridTransform::Start() {

}

void eng::cpt::GridTransform::Update() {
	auto f_TransPos{ GetOwner().GetTransform().GetGlobal().position };
	m_Position.x = static_cast<int>(std::round(f_TransPos.x));
	m_Position.y = static_cast<int>(std::round(f_TransPos.y));
}

void eng::cpt::GridTransform::OnEvent(Event event) {
}
