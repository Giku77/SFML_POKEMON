#include "stdafx.h"
#include "ScenePokeDev.h"
#include "Button.h"
#include "Text.h"

ScenePokeDev::ScenePokeDev()
	: Scene(SceneIds::Dev3)
{
}


void ScenePokeDev::Init()
{
	texIds.push_back("graphics/18507.png");
	fontIds.push_back("fonts/Galmuri11-Bold.ttf");

	button = new Button("TestButton");
	button->SetOnClick([=]() {
		button->SetString(L"메시지를 클릭해서 다음 메시지를 출력했어요.");
		});
	button->SetButton({ FRAMEWORK.GetWindowSize().x / 2.f + 200.f, 300.f }, sf::Color::Black, "fonts/Galmuri11-Bold.ttf");
	button->AddButton(L"메시지를 눌러보세요.", 30, sf::Color::White);
	sf::Vector2f pos = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y - 120.f };
	//sf::Vector2f pos = { 0.f, FRAMEWORK.GetWindowSize().y - 60.f };
	button->SetPosition(ScreenToUi((sf::Vector2i)pos));
	UI_MGR.Add(button);

	text = new Text("Test");
	sf::Vector2f pos2 = { FRAMEWORK.GetWindowSize().x / 2.f, 50.f };

	text->SetBackGround("graphics/18507.png");
	text->SetFillColor(sf::Color::Black);
	text->AddText("fonts/Galmuri11-Bold.ttf", L"포켓몬스터", 30, ScreenToUi((sf::Vector2i)pos2));
	UI_MGR.Add(text);

	Scene::Init();
}

void ScenePokeDev::Enter()
{
	Scene::Enter();
}

void ScenePokeDev::Update(float dt)
{
	Scene::Update(dt);
}

void ScenePokeDev::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	UI_MGR.Draw(window);
}