#include "stdafx.h"
#include "Text.h"
#include "TextWriter.h"

Text::Text(const std::string& name, bool b)
	: UI(name), isWriter(b)
{
}
void Text::SetString(const sf::String& str)
{
	if (isWriter)
	{
		delete writer;
		writer = nullptr;
		writer = new TextWriter(&text, str);
	}
	else text.setString(str);
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

void Text::SetOrigin(const Origins& o)
{
	Utils::SetOrigin(text, o);
}

std::string Text::GetString() const
{
	return text.getString();
}

void Text::SetBackGround(const std::string& tx)
{
	isBg = true;
	texId = tx;
}

void Text::SetBackGround(const std::string& tx, const sf::IntRect& i, float w, float h)
{
	isBg = true;
	texId = tx;
	intrt = i;
	bg.setScale(w, h);
}

void Text::AddText(const std::string& fontId, const sf::String& text, unsigned int size, const sf::Vector2f& v)
{
	if (isWriter)
	{
		delete writer;
		writer = nullptr;
	}
	writer = new TextWriter(&this->text, text);
	this->fontId = fontId;
	SetString(text);
	SetCharacterSize(size);
	SetPosition(v);
}

void Text::SetPosition(const sf::Vector2f& pos)
{
	sf::Vector2f p;
	p.x = pos.x - text.getLocalBounds().width / 4.f;
	p.y = pos.y - text.getLocalBounds().height / 4.f;
	UI::SetPosition(pos);
	text.setPosition(pos);
	if (isBg) bg.setPosition(pos);
}

void Text::Init()
{
}

void Text::Release()
{
}

void Text::Reset()
{
	//isBg = false;
	font.loadFromFile(fontId);
	tex.loadFromFile(texId);
	text.setFont(font);
	bg.setTexture(tex);
	if (intrt.width != 0 && intrt.height != 0) bg.setTextureRect(intrt);
	Utils::SetOrigin(text, Origins::MC);
	Utils::SetOrigin(bg, Origins::MC);
}

void Text::Update(float dt)
{
	if (isWriter) {
		writer->Update(dt);
	}
}

void Text::Draw(sf::RenderWindow& window)
{
	if(isBg) window.draw(bg);
	window.draw(text);
}
