#include "stdafx.h"
#include "SceneBattle.h"
#include "Text.h"
#include "Button.h"
#include "UiMgr.h"
#include "SpriteAnimator.h"
#include "MoveDB.h"
#include "BattleMgr.h"
#include "BattlePokemon.h"


BattleMgr bmgr;

SceneBattle::SceneBattle()
	: Scene(SceneIds::Battle)
{
}

SceneBattle::~SceneBattle()
{
	delete uiMgr;
	uiMgr = nullptr;
	delete pokeName1;
	pokeName1 = nullptr;
}

void SceneBattle::Init()
{
	Scene::Init();
	uiMgr = new UiMgr();
	MoveDB::Instance().LoadFromJson("data/moves_korean_kr_full.json");
	tex.loadFromFile("graphics/battelBackGround.png");
	sprite.setTexture(tex);
	sprite.setScale(4.3f, 7.f);

	pokeName1 = new Text("pokename1");
	sf::Vector2f pos1 = { 400.f, FRAMEWORK.GetWindowSize().y / 2.f - 250.f};

	pokeName1->SetBackGround("graphics/battle_Sprite.png", { 0, 0, 112, 35 }, 5.f, 5.f);
	pokeName1->SetFillColor(sf::Color::Black);
	pokeName1->AddText("fonts/pokemon-dppt.otf", "Name1", 40, ScreenToUi((sf::Vector2i)pos1));
	pokeName1->GetText().setPosition({ pokeName1->GetPosition().x - 200.f, pokeName1->GetPosition().y - 40.f });
	uiMgr->Add(pokeName1);

	pokeName2 = new Text("pokename2");
	sf::Vector2f pos2 = { FRAMEWORK.GetWindowSize().x - 300.f, FRAMEWORK.GetWindowSize().y / 2.f + 100.f };

	pokeName2->SetBackGround("graphics/battle_Sprite.png", { 0, 41, 112, 41 }, 5.f, 5.f);
	pokeName2->SetFillColor(sf::Color::Black);
	pokeName2->AddText("fonts/pokemon-dppt.otf", "Name2", 40, ScreenToUi((sf::Vector2i)pos2));
	pokeName2->GetText().setPosition({ pokeName2->GetPosition().x - 160.f, pokeName2->GetPosition().y - 60.f });
	uiMgr->Add(pokeName2);

	battleMsg = new Text("battleMsg");
	sf::Vector2f pos3 = { FRAMEWORK.GetWindowSize().x - 300.f, FRAMEWORK.GetWindowSize().y - 80.f };

	battleMsg->SetBackGround("graphics/battle_Sprite.png", { 204, 53, 539, 53 }, 5.f, 5.f);
	battleMsg->SetFillColor(sf::Color::Black);
	battleMsg->AddText("fonts/pokemon-dppt.otf", L"무엇을 할까?", 40, ScreenToUi((sf::Vector2i)pos3));
	battleMsg->GetText().setPosition({ 100.f, battleMsg->GetPosition().y - 60.f });
	uiMgr->Add(battleMsg);


	mov1 = new Button("mov1");
	mov1->isMouseOverColor = true;
	mov1->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo1Id = Utils::RandomRange(1, 100);
	mov1->AddButton(MoveDB::Instance().GetMove(mo1Id)->kname + "\n" + std::to_string(MoveDB::Instance().GetMove(mo1Id)->pp), 40, sf::Color::Black);
	sf::Vector2f mpos1 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov1->SetPosition(ScreenToUi((sf::Vector2i)mpos1));
	mov1->SetOutlineColor(sf::Color::Black);
	mov1->SetOutlineThickness(4.f);
	mov1->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos1.x - 110.f, mpos1.y - 15.f)));
	//mov1->SetOnClick([=]() {bmgr.UseMove(mo1Id); });
	uiMgr->Add(mov1);

	mov2 = new Button("mov2");
	mov2->isMouseOverColor = true;
	mov2->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo2Id = Utils::RandomRange(1, 100);
	mov2->AddButton(MoveDB::Instance().GetMove(mo2Id)->kname + "\n" + std::to_string(MoveDB::Instance().GetMove(mo2Id)->pp), 40, sf::Color::Black);
	sf::Vector2f mpos2 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov2->SetPosition(ScreenToUi((sf::Vector2i)mpos2));
	mov2->SetOutlineColor(sf::Color::Black);
	mov2->SetOutlineThickness(4.f);
	mov2->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos2.x - 110.f, mpos2.y - 15.f)));
	uiMgr->Add(mov2);

	mov3 = new Button("mov3");
	mov3->isMouseOverColor = true;
	mov3->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo3Id = Utils::RandomRange(1, 100);
	mov3->AddButton(MoveDB::Instance().GetMove(mo3Id)->kname + "\n" + std::to_string(MoveDB::Instance().GetMove(mo3Id)->pp), 40, sf::Color::Black);
	sf::Vector2f mpos3 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov3->SetPosition(ScreenToUi((sf::Vector2i)mpos3));
	mov3->SetOutlineColor(sf::Color::Black);
	mov3->SetOutlineThickness(4.f);
	mov3->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos3.x - 110.f, mpos3.y - 15.f)));
	uiMgr->Add(mov3);

	mov4 = new Button("mov4");
	mov4->isMouseOverColor = true;
	mov4->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo4Id = Utils::RandomRange(1, 100);
	mov4->AddButton(MoveDB::Instance().GetMove(mo4Id)->kname + "\n" + std::to_string(MoveDB::Instance().GetMove(mo4Id)->pp), 40, sf::Color::Black);
	sf::Vector2f mpos4 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov4->SetPosition(ScreenToUi((sf::Vector2i)mpos4));
	mov4->SetOutlineColor(sf::Color::Black);
	mov4->SetOutlineThickness(4.f);
	mov4->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos4.x - 110.f, mpos4.y - 15.f)));
	uiMgr->Add(mov4);
	uiMgr->Init();


	sf::Vector2f pos22 = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f };


	pokTex1.loadFromFile("graphics/ball-.png");
	pokSprite1.setTexture(pokTex1);
	pokSprite1.setPosition(ScreenToUi((sf::Vector2i)(pos22 - sf::Vector2f(500.f, 200.f))));
	pokSprite1.setScale(1.f, 1.f);

	std::vector<sf::IntRect> rects = {
		{1, 36, 78, 70},
		{82, 36, 78, 70}
	};

	pokTex1 = Utils::loadWithColorKey("graphics/starting.png", sf::Color(147, 187, 236, 255));
	pokSprite1.setTexture(pokTex1);
	pokSprite1.setPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(750.f, 130.f)));
	pokSprite1.setScale(5.f, 5.f);
	poke1 = new SpriteAnimator(&pokSprite1, rects, 0.35f);

	pokTex2 = Utils::loadWithColorKey("graphics/starting.png", sf::Color(147, 187, 236, 255));
	pokSprite2.setTexture(pokTex2);
	pokSprite2.setTextureRect({ 164, 34, 77, 70 });
	pokSprite2.setPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(150.f, 310.f)));
	pokSprite2.setScale(5.f, 5.f);
	
	//PlayerPokemon p(Pokemon{ "Pikachu", 35, 55, 40, {...} });
	//EnemyPokemon  e(Pokemon{ "Charmander", 34, 52, 43, {...} });

	//bmgr.Init(&p, &e);
}

void SceneBattle::Enter()
{
	Scene::Enter();
}

void SceneBattle::Update(float dt)
{
	Scene::Update(dt);
	uiMgr->Update(dt);
	poke1->Update(dt, true);
	if(InputMgr::GetKeyDown(sf::Keyboard::LShift)) {
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}

void SceneBattle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(sprite);
	window.draw(pokSprite2);
	uiMgr->Draw(window);
	window.draw(pokSprite1);
}