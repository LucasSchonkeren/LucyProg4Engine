#pragma once
#include <string>

struct _TTF_Font;
namespace dae
{

/**
* Simple RAII wrapper for a _TTF_Font
*/
class Font final
{
public:
	explicit Font(const std::string& fullPath, unsigned int size);
	explicit Font(_TTF_Font* font);

	~Font();

	Font			(const Font&)	= delete;
	Font& operator= (const Font&)	= delete;
	Font			(Font&&)		= delete;
	Font& operator= (const Font&&)	= delete;

public:
	_TTF_Font* GetFont() const;
		
private:
	_TTF_Font* m_font;
};

} // !dae
