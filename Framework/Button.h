#pragma once
#include "UI.h"

class TextWriter;
class Button : public UI
{
protected:
	sf::Font font;
	sf::Texture tex;
	sf::RectangleShape box;
	sf::Sprite bg;
	sf::Text text;
	std::string fontId;
	std::string texId;
	sf::Vector2f vec2;
	sf::Color buttonColor;
	sf::Color MouseOverbuttonColor;

	std::function<void()> onClick;
	TextWriter* writer = nullptr;
	bool isBg = false;
	bool isWriter = false;

public:
	bool isMouseOverColor = false;

	Button(const std::string& name = "", bool b = false);
	virtual ~Button() = default;

	const sf::Vector2f getBoxPos() const { return box.getPosition(); }
	void ButtonSetFillColor(const sf::Color& color);
	void TextSetFillColor(const sf::Color& color);
	void SetOutlineColor(sf::Color c);
	void SetOutlineThickness(float f);
	void SetSize(sf::Vector2f v);

	const std::string& GetFontId() const { return fontId; }
	void SetFontId(const std::string& fId) { fontId = fId; }

	void SetBackGroud(const sf::String& str, float w, float h) {
		isBg = true;
		texId = str;
		bg.setScale(w, h);
	}
	const sf::Sprite& getBg() const { return bg; }

	void TextSetPosition(const sf::Vector2f& pos);
	void SetString(const sf::String& str);
	void SetCharacterSize(unsigned int size);
	std::string GetString() const;

	sf::Text& GetText() { return text; }
	const sf::Text& GetText() const { return text; }


	void SetButton(const sf::Vector2f& v, const sf::Color& c, const std::string& f, const sf::Color& oc = sf::Color::White, float t = 3.f);
	void AddButton(const sf::String& str, unsigned int size, const sf::Color& color);

	void SetOnClick(std::function<void()> func);

	void SetPosition(const sf::Vector2f& pos) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

