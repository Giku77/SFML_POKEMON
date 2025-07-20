#include "stdafx.h"
#include "SceneEnd.h"
#include "SpriteAnimator.h"
#include "Text.h"

SceneEnd::SceneEnd()
	: Scene(SceneIds::End)
{
}

SceneEnd::~SceneEnd()
{
	delete end;
	end = nullptr;
	delete endText;
	endText = nullptr;
}

void SceneEnd::Init()
{
	newTex.loadFromFile("graphics/ENDING.png");
	newScreen.setTexture(newTex);
	sf::Vector2f pos = { 0.f,  0.f };
	newScreen.setPosition(ScreenToUi((sf::Vector2i)pos));
	newScreen.setScale(2.f, 2.7f);
	end = new SpriteAnimator(&newScreen, newTex, 500.f, 375.f, 0.03f);

	endText = new Text();
	endText->SetFillColor(sf::Color::Black);
	endText->AddText("fonts/pokemon-dppt.otf", L"그리고 그들의 이야기는... 계속된다.\nfin.", 50, ScreenToUi(sf::Vector2i(450, 100)));
	endText->Reset();
	Scene::Init();
}

void SceneEnd::Enter()
{
	Scene::Enter();
}

void SceneEnd::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(newScreen);
	endText->Draw(window);
}

void SceneEnd::Update(float dt)
{
	Scene::Update(dt);
	endText->Update(dt);
	if (end->getIndex() == 66) {
		end->setIndex(0);
	}

	end->Update(dt, true);
}