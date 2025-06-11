#include "GridTransform.h"
#include "../Actor.h"

glm::vec2 eng::cpt::GridTransform::GetAllignment() {
	return glm::vec2{};
}

void eng::cpt::GridTransform::AllignWithGrid() {
	glm::vec2 f_NewPos{
		(m_Position.x + 0.5f)* m_GridPtr->GetPixelWidth(),
		(m_Position.y + 0.5f) * m_GridPtr->GetPixelWidth(),
	};

	f_NewPos += m_GridPtr->Owner().GetTransform().GetGlobal().position;

	m_Position = f_NewPos;
}

void eng::cpt::GridTransform::OnEnable() {
	m_GridPtr = Grid::FindParentGrid(Owner());
	UpdatePosition();
}

void eng::cpt::GridTransform::Start() {
	Owner().GetTransform().AddObserver(*this);
}

void eng::cpt::GridTransform::Update() {
	if (m_DirtyPosition) UpdatePosition();
}

void eng::cpt::GridTransform::OnDisable() {
	m_GridPtr = nullptr;
}

void eng::cpt::GridTransform::OnEvent(Event event) {
	if (event.eventTypeHash != eng::eventHash::PositionChanged) return;
	m_DirtyPosition = true;
}

void eng::cpt::GridTransform::UpdatePosition() {
	assert(m_GridPtr != nullptr);

	const glm::vec2 f_GlobalPos{ Owner().GetTransform().GetGlobal().position + m_GridPtr->Owner().GetTransform().GetGlobal().position};
	const float f_GridSize{m_GridPtr->GetPixelWidth()};

	m_Position.x = static_cast<int>(f_GlobalPos.x / f_GridSize);
	m_Position.y = static_cast<int>(f_GlobalPos.x / f_GridSize);
}
