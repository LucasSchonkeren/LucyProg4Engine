#include "Transform.h"

namespace cpt {
void Transform::SetLocalPosition(float x, float y) {
	m_TransformData.position.x = x;
	m_TransformData.position.y = y;
}
eng::TransformData const& Transform::GetLocal() const
{
	return m_TransformData;
}

} // !cpt