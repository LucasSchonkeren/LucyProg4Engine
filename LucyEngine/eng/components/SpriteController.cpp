#include "SpriteController.h"
#include "../Actor.h"

void eng::cpt::SpriteController::Start() {
	m_TextureRendererPtr = Owner().GetComponent<TextureRenderer>();
}

void eng::cpt::SpriteController::Update() {
	m_SecLeftThisFrame -= Owner().DeltaTime();
	if (m_SecLeftThisFrame > 0) return;

	m_SecLeftThisFrame = m_SecPerFrame;
	++m_CurrentFrame;
	if (m_CurrentFrame >= m_SpriteSourcePositions[m_CurrentSprite].size()) m_CurrentFrame = 0;

	auto f_Pos{ m_SpriteSourcePositions[m_CurrentSprite][m_CurrentFrame]};
	m_TextureRendererPtr->SetSourceRect({f_Pos.x, f_Pos.y, m_Size.x, m_Size.y});
}

void eng::cpt::SpriteController::SetSprite(const std::string& spriteName) {
	m_CurrentSprite = spriteName;
}
