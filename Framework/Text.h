#pragma once
#include "UI.h"
class Text : public UI
{
protected:
	sf::Sprite bg;
	sf::Text text;
	std::string texId;
	std::string fontId;

	bool isBg = false;

public:
	Text(const std::string& name = "");
	virtual ~Text() = default;

	const std::string& GetFontId() const { return fontId; }
	void SetFontId(const std::string& fId) { fontId = fId; }

	void SetString(const sf::String& str);
	void SetCharacterSize(unsigned int size);
	void SetFillColor(const sf::Color& color);
	std::string GetString() const;

	sf::Text& GetText() { return text; }
	const sf::Text& GetText() const { return text; }

	void SetBackGround(const std::string& tx);

	void AddText(const std::string& fontId, const sf::String& text, unsigned int size, const sf::Vector2f& v);

	void SetPosition(const sf::Vector2f& pos) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

