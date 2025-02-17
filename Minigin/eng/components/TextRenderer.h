#pragma once

#include "../AbstractComponent.h"
#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"

#include <string>

namespace cpt {

class TextRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	TextRenderer(const std::string& text = "", const std::string& fontPath = "Lingua.otf", unsigned int size = 10);
	~TextRenderer()	= default;

	TextRenderer(const TextRenderer& other) : 
		m_NeedsUpdate(true), m_Text(other.m_Text) {
		m_TextTextureUptr = other.m_TextTextureUptr ? std::make_unique<dae::Texture2D>(other.m_TextTextureUptr->GetSDLTexture()) : nullptr;
		m_FontUptr = other.m_FontUptr ? std::make_unique<dae::Font>(other.m_FontUptr->GetFont()) : nullptr;
	}
	TextRenderer& operator=	(const TextRenderer&)	= default;

	TextRenderer				( TextRenderer&&)	= default;
	TextRenderer& operator=	( TextRenderer&&)	= default;

public: //------------------|General methods|--------------------------

	void SetText(const std::string& text);

public: //------------------|Gameloop methods|--------------------------

	void Update() override;
	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	std::string m_Text;
	bool m_NeedsUpdate{};
	u_ptr<dae::Font> m_FontUptr;
	u_ptr<dae::Texture2D> m_TextTextureUptr{};
}; // !TextRenderer

} // !cpt