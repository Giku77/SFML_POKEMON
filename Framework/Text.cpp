#include "stdafx.h"
#include "Text.h"

Text::Text(const std::string& name)
	: UI(name)
{
}
void Text::SetString(const sf::String& str)
{
	text.setString(str);
	Utils::SetOrigin(text, Origins::MC);
}

void Text::SetCharacterSize(unsigned int size)
{
	text.setCharacterSize(size);
	Utils::SetOrigin(text, Origins::MC);
}

void Text::SetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

std::string Text::GetString() const
{
	return text.getString();
}

void Text::SetBackGround(const std::string& tx)
{
	isBg = true;
	tex.loadFromFile(tx);
	bg.setTexture(tex);
}

void Text::SetPosition(const sf::Vector2f& pos)
{
	UI::SetPosition(pos);
	text.setPosition(pos);
}

void Text::Init()
{
}

void Text::Release()
{
}

void Text::Reset()
{
	isBg = false;
	text.setFont(FONT_MGR.Get(fontId));
	Utils::SetOrigin(text, Origins::MC);
	Utils::SetOrigin(bg, Origins::MC);
}

void Text::Update(float dt)
{
}

void Text::Draw(sf::RenderWindow& window)
{
	if (isBg) window.draw(bg);
	window.draw(text);
}
