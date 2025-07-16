#include "stdafx.h"
#include "SceneBattle.h"
#include "Text.h"
#include "Button.h"
#include "UiMgr.h"
#include "SpriteAnimator.h"
#include "MoveDB.h"
#include "BattleMgr.h"
#include "BattlePokemon.h"
#include "PokemonDB.h"


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

void SceneBattle::CreateMoveAndPokemon()
{
	PokemonDB::Instance().LoadFromJson("data/_pokemon.json");
	PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");
	MoveDB::Instance().LoadFromJson("data/moves_korean_kr_full.json");
	ePoke = PokemonDB::Instance().GetPokemon(Utils::RandomRange(1, 51));
	mPoke = PokemonDB::Instance().GetMyPokemon();
	if (!ePoke) {
		std::cerr << "적 포켓몬이 없습니다!" << std::endl;
		return;
	}
	if (!mPoke) {
		std::cerr << "내 포켓몬이 없습니다!" << std::endl;
		return;
	}
	eHp = ePoke->hp;
	mHp = mPoke->hp;
	for (int i = 0; i < 8; ++i) {
		int moId = Utils::RandomRange(1, 100);
		Move m;
		const MoveData* db = MoveDB::Instance().GetMove(moId);
		m.id = db->id;
		m.name = db->kname;
		m.power = db->power;
		m.pp = db->pp;
		m.accuracy = db->accuracy;
		m.type = db->type;
		if (i < 3) { 
			ePoke->moves.push_back(m);
		} else mPoke->moves.push_back(m);
	}
	bmgr.Init((PlayerPokemon*)mPoke, (EnemyPokemon*)ePoke);
}

void SceneBattle::Init()
{
	Scene::Init();
	uiMgr = new UiMgr();
	CreateMoveAndPokemon();
	tex.loadFromFile("graphics/battelBackGround.png");
	sprite.setTexture(tex);
	sprite.setScale(4.3f, 7.f);

	pokeName1 = new Text("pokename1");
	sf::Vector2f pos1 = { 400.f, FRAMEWORK.GetWindowSize().y / 2.f - 250.f};

	pokeName1->SetBackGround("graphics/battle_Sprite.png", { 0, 0, 112, 35 }, 5.f, 5.f);
	pokeName1->SetFillColor(sf::Color::Black);
	pokeName1->AddText("fonts/pokemon-dppt.otf", ePoke->name, 40, ScreenToUi((sf::Vector2i)pos1));
	pokeName1->GetText().setPosition({ pokeName1->GetPosition().x - 190.f, pokeName1->GetPosition().y - 40.f });
	uiMgr->Add(pokeName1);

	pokeLv1 = new Text("pokeLv1");
	pokeLv1->SetFillColor(sf::Color::Black);
	pokeLv1->AddText("fonts/pokemon-dppt.otf", "5", 50, ScreenToUi((sf::Vector2i)(pos1 + sf::Vector2f(455.f, -7.f))));
	pokeLv1->GetText().setPosition({ pokeLv1->GetPosition().x - 200.f, pokeLv1->GetPosition().y - 40.f });
	uiMgr->Add(pokeLv1);

	pokeBackHp1.setSize({ 242.f, 20.f });
	pokeBackHp1.setFillColor(sf::Color(248, 248, 248));
	pokeBackHp1.setPosition(ScreenToUi((sf::Vector2i)(pos1 + sf::Vector2f(-90.f, 7.f))));

	pokeHp1.setSize({ 242.f, 20.f });
	pokeHp1.setFillColor(sf::Color(160, 0, 233));
	pokeHp1.setPosition(ScreenToUi((sf::Vector2i)(pos1 + sf::Vector2f(-90.f, 7.f))));

	pokeName2 = new Text("pokename2");
	sf::Vector2f pos2 = { FRAMEWORK.GetWindowSize().x - 300.f, FRAMEWORK.GetWindowSize().y / 2.f + 100.f };

	pokeName2->SetBackGround("graphics/battle_Sprite.png", { 0, 41, 112, 41 }, 5.f, 5.f);
	pokeName2->SetFillColor(sf::Color::Black);
	pokeName2->AddText("fonts/pokemon-dppt.otf", mPoke->name, 40, ScreenToUi((sf::Vector2i)pos2));
	pokeName2->GetText().setPosition({ pokeName2->GetPosition().x - 135.f, pokeName2->GetPosition().y - 60.f });
	uiMgr->Add(pokeName2);

	pokeLv2 = new Text("pokeLv2");
	pokeLv2->SetFillColor(sf::Color::Black);
	pokeLv2->AddText("fonts/pokemon-dppt.otf", "5", 50, ScreenToUi((sf::Vector2i)(pos2 + sf::Vector2f(515.f, -17.f))));
	pokeLv2->GetText().setPosition({ pokeLv2->GetPosition().x - 200.f, pokeLv2->GetPosition().y - 40.f });
	uiMgr->Add(pokeLv2);

	pokeBackHp2.setSize({ 242.f, 20.f });
	pokeBackHp2.setFillColor(sf::Color(248, 248, 248));
	pokeBackHp2.setPosition(ScreenToUi((sf::Vector2i)(pos2 + sf::Vector2f(-35.f, -3.f))));

	pokeHp2.setSize({ 242.f, 20.f });
	pokeHp2.setFillColor(sf::Color(160, 0, 233));
	pokeHp2.setPosition(ScreenToUi((sf::Vector2i)(pos2 + sf::Vector2f(-35.f, -3.f))));

	battleMsg = new Text("battleMsg", true);
	bmgr.SetText(battleMsg);
	sf::Vector2f pos3 = { FRAMEWORK.GetWindowSize().x - 300.f, FRAMEWORK.GetWindowSize().y - 80.f };

	battleMsg->SetBackGround("graphics/battle_Sprite.png", { 204, 53, 539, 53 }, 5.f, 5.f);
	battleMsg->SetFillColor(sf::Color::Black);
	battleMsg->AddText("fonts/pokemon-dppt.otf", L"무엇을 할까?", 40, ScreenToUi((sf::Vector2i)pos3));
	battleMsg->GetText().setPosition({ 10.f, battleMsg->GetPosition().y - 60.f });
	uiMgr->Add(battleMsg);


	mov1 = new Button("mov1");
	mov1->isMouseOverColor = true;
	mov1->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo1Id = Utils::RandomRange(1, 100);
	mov1->AddButton(sf::String(mPoke->moves[0].name) + "\n" + std::to_string(mPoke->moves[0].pp), 40, sf::Color::Black);
	sf::Vector2f mpos1 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov1->SetPosition(ScreenToUi((sf::Vector2i)mpos1));
	mov1->SetOutlineColor(sf::Color::Black);
	mov1->SetOutlineThickness(4.f);
	mov1->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos1.x - 110.f, mpos1.y - 15.f)));
	mov1->SetOnClick([=]() {bmgr.UseMove(mPoke->moves[0].id); });
	uiMgr->Add(mov1);

	mov2 = new Button("mov2");
	mov2->isMouseOverColor = true;
	mov2->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo2Id = Utils::RandomRange(1, 100);
	mov2->AddButton(sf::String(mPoke->moves[1].name) + "\n" + std::to_string(mPoke->moves[1].pp), 40, sf::Color::Black);
	sf::Vector2f mpos2 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov2->SetPosition(ScreenToUi((sf::Vector2i)mpos2));
	mov2->SetOutlineColor(sf::Color::Black);
	mov2->SetOutlineThickness(4.f);
	mov2->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos2.x - 110.f, mpos2.y - 15.f)));
	mov2->SetOnClick([=]() {bmgr.UseMove(mPoke->moves[1].id); });
	uiMgr->Add(mov2);

	mov3 = new Button("mov3");
	mov3->isMouseOverColor = true;
	mov3->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo3Id = Utils::RandomRange(1, 100);
	mov3->AddButton(sf::String(mPoke->moves[2].name) + "\n" + std::to_string(mPoke->moves[2].pp), 40, sf::Color::Black);
	sf::Vector2f mpos3 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov3->SetPosition(ScreenToUi((sf::Vector2i)mpos3));
	mov3->SetOutlineColor(sf::Color::Black);
	mov3->SetOutlineThickness(4.f);
	mov3->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos3.x - 110.f, mpos3.y - 15.f)));
	mov3->SetOnClick([=]() {bmgr.UseMove(mPoke->moves[2].id); });
	uiMgr->Add(mov3);

	mov4 = new Button("mov4");
	mov4->isMouseOverColor = true;
	mov4->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo4Id = Utils::RandomRange(1, 100);
	mov4->AddButton(sf::String(mPoke->moves[3].name) + "\n" + std::to_string(mPoke->moves[3].pp), 40, sf::Color::Black);
	sf::Vector2f mpos4 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov4->SetPosition(ScreenToUi((sf::Vector2i)mpos4));
	mov4->SetOutlineColor(sf::Color::Black);
	mov4->SetOutlineThickness(4.f);
	mov4->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos4.x - 110.f, mpos4.y - 15.f)));
	mov4->SetOnClick([=]() {bmgr.UseMove(mPoke->moves[3].id); });
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
	bmgr.Update(dt);
	poke1->Update(dt, true);
	if(bmgr.BattleOver()) {
		overTime += dt;
		if (overTime > 2.f) {
			SCENE_MGR.ChangeScene(SceneIds::Game);
			overTime = 0.f;
		}
	}

	float maxHpWidth = 242.f;


	int ecurrentHp = ePoke->hp;
	int mcurrentHp = mPoke->hp;

	float ehpRatio = static_cast<float>(ecurrentHp) / eHp;
	if (ehpRatio < 0.f) ehpRatio = 0.f;
	if (ehpRatio > 1.f) ehpRatio = 1.f;

	float mhpRatio = static_cast<float>(mcurrentHp) / mHp;
	if (mhpRatio < 0.f) mhpRatio = 0.f;
	if (mhpRatio > 1.f) mhpRatio = 1.f;

	pokeHp1.setSize({ maxHpWidth * ehpRatio, 20.f });
	pokeHp2.setSize({ maxHpWidth * mhpRatio, 20.f });
}

void SceneBattle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(sprite);
	window.draw(pokSprite2);
	uiMgr->Draw(window);
	window.draw(pokSprite1);
	window.draw(pokeBackHp1);
	window.draw(pokeHp1);
	window.draw(pokeBackHp2);
	window.draw(pokeHp2);
}