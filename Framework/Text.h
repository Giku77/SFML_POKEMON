#pragma once
#include "UI.h"

class TextWriter;
class Text : public UI
{
protected:
	sf::Sprite bg;
	sf::Text text;
	sf::IntRect intrt;
	std::string texId;
	std::string fontId;

	TextWriter* writer = nullptr;
	sf::Font font;
	sf::Texture tex;

	bool isBg = false;
	bool isWriter = false;

public:
	Text(const std::string& name = "", bool iswriter = false);
	virtual ~Text() = default;

	const std::string& GetFontId() const { return fontId; }
	void SetFontId(const std::string& fId) { fontId = fId; }

	void SetString(const sf::String& str);
	void SetCharacterSize(unsigned int size);
	void SetFillColor(const sf::Color& color);
	void SetOrigin(const Origins& o);
	std::string GetString() const;

	sf::Text& GetText() { return text; }
	const sf::Text& GetText() const { return text; }

	void SetBackGround(const std::string& tx);
	void SetBackGround(const std::string& tx, const sf::IntRect& i, float w, float h);

	void AddText(const std::string& fontId, const sf::String& text, unsigned int size, const sf::Vector2f& v);

	void SetPosition(const sf::Vector2f& pos) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

