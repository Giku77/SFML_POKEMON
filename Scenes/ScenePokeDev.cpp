#include "stdafx.h"
#include "ScenePokeDev.h"
#include "Button.h"

ScenePokeDev::ScenePokeDev()
	: Scene(SceneIds::Dev3)
{
}

void ScenePokeDev::Init()
{
	fontIds.push_back("fonts/Galmuri11-Bold.ttf");

	button = new Button("TestButton");
	button->SetButton({ 100.f, 100.f }, sf::Color::Black, "fonts/Galmuri11-Bold.ttf");
	button->AddButton(L"¹öÆ°", 20, sf::Color::White);
	sf::Vector2f pos = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f };
	button->SetPosition(ScreenToUi((sf::Vector2i)pos));
	UI_MGR.Add(button);



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