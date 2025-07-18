#include "stdafx.h"
#include "Button.h"
#include "TextWriter.h"

Button::Button(const std::string& name, bool b)
	: UI(name), isWriter(b)
{
}

void Button::ButtonSetFillColor(const sf::Color& color)
{
	buttonColor = color;
	MouseOverbuttonColor = color;
	box.setFillColor(color);
}

void Button::SetOutlineColor(sf::Color c)
{
	box.setOutlineColor(c);
}

void Button::SetOutlineThickness(float f)
{
	box.setOutlineThickness(f);
}

void Button::SetSize(sf::Vector2f v)
{
	vec2 = v;
	box.setSize(v);
}

void Button::SetButton(const sf::Vector2f& v, const sf::Color& c, const std::string& f, const sf::Color& oc, float t)
{
	SetSize(v);
	ButtonSetFillColor(c);
	SetFontId(f);
	SetOutlineColor(oc);
	SetOutlineThickness(t);
}

void Button::AddButton(const sf::String& str, unsigned int size, const sf::Color& color)
{
	if (isWriter)
	{
		delete writer;
		writer = nullptr;
	}
	writer = new TextWriter(&text, str);
	if(str != "") SetString(str);
	SetCharacterSize(size);
	TextSetFillColor(color);
}

void Button::SetOnClick(std::function<void()> func)
{
	onClick = func;
}

void Button::SetPosition(const sf::Vector2f& pos)
{
	UI::SetPosition(pos);
	box.setPosition(pos);
	bg.setPosition(pos);
	sf::Vector2f p;
	p.x = pos.x - text.getCharacterSize() / 4.f;
	p.y = pos.y - text.getCharacterSize() / 4.f;
	text.setPosition(p);
}

void Button::TextSetPosition(const sf::Vector2f& pos)
{
	text.setPosition(pos);
}

void Button::SetString(const sf::String& str)
{
	if (isWriter)
	{
		delete writer;
		writer = nullptr;
		writer = new TextWriter(&text, str);
	}
	else text.setString(str);
	Utils::SetOrigin(text, Origins::ML);
}

void Button::SetCharacterSize(unsigned int size)
{
	text.setCharacterSize(size);
}

void Button::TextSetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

std::string Button::GetString() const
{
	return text.getString();
}

void Button::Init()
{
}

void Button::Release()
{
}

void Button::Reset()
{
	font.loadFromFile(fontId);
	tex.loadFromFile(texId);
	text.setFont(font);
	bg.setTexture(tex);
	Utils::SetOrigin(bg, Origins::MC);
	Utils::SetOrigin(text, Origins::ML);
	Utils::SetOrigin(box, Origins::MC);
}

void Button::Update(float dt)
{
	sf::Vector2f mouseWorldPos = FRAMEWORK.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(FRAMEWORK.GetWindow()));
	bool isMouseOver = false;
	if(isBg) isMouseOver = Utils::PointInTransformBounds(bg, bg.getLocalBounds(), mouseWorldPos);
	else isMouseOver = Utils::PointInTransformBounds(box, box.getLocalBounds(), mouseWorldPos);

	if (isMouseOver) {
		if (isMouseOverColor) {
			if (buttonColor == MouseOverbuttonColor) {
				MouseOverbuttonColor.r -= 10;
				MouseOverbuttonColor.g -= 10;
				MouseOverbuttonColor.b -= 10;
				MouseOverbuttonColor.a = 100;
			}
			box.setFillColor(MouseOverbuttonColor);
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left)) {
			std::cout << "버튼 클릭 : " << GetName() << std::endl;
			onClick();
		}
	}
	else ButtonSetFillColor(buttonColor);

	if (InputMgr::GetKeyDown(sf::Keyboard::Space) && texId == "graphics/18507.png") {
		std::cout << "버튼 스페이스 : " << GetName() << std::endl;
		onClick();
	}

	if (isWriter) {
		writer->Update(dt);
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	if (GetActive()) {
		if(isBg) window.draw(bg);
		else window.draw(box);
		window.draw(text);
	}
}