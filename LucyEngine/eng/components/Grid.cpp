#include "Grid.h"

eng::cpt::Grid* eng::cpt::Grid::FindParentGrid(Actor& actor) {
    Actor* f_ParentPtr{ actor.GetParent() };
    if (f_ParentPtr == nullptr) return nullptr;

    Grid* f_GridPtr{ f_ParentPtr->GetComponent<Grid>() };
    if (f_GridPtr == nullptr) return FindParentGrid(*f_ParentPtr);

    return f_GridPtr;
}

glm::vec2 eng::cpt::Grid::GridToWorldPosition(glm::ivec2 gridPosition) {
    auto f_Offset{ Owner().GetTransform().GetGlobal().position };
    return {
        (gridPosition.x + 0.5f) * GetPixelWidth() + f_Offset.x,
        (gridPosition.y + 0.5f) * GetPixelWidth() + f_Offset.y
    };
}

float eng::cpt::Grid::GetPixelWidth() const {
    return m_PixelWidth;
}

void eng::cpt::Grid::OnEvent(Event event) {

}
