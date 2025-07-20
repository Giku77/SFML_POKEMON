#include "stdafx.h"
#include "SceneBattle.h"
#include "Text.h"
#include "Button.h"
#include "SpriteAnimator.h"
#include "MoveDB.h"
#include "BattlePokemon.h"
#include "PokemonDB.h"



SceneBattle::SceneBattle()
	: Scene(SceneIds::Battle)
{
}

SceneBattle::~SceneBattle()
{
	delete poke1; 
	poke1 = nullptr;
}

std::vector<sf::IntRect> GetPokemonRects(int pokemonId, const std::vector<int>& frameCounts)
{
	const int cellSize = 80;
	const int topOffset = 34;
	const int sheetCols = 30;  // ���� �� ����
	const int framesPerRow = sheetCols; // ���� �� ���� ĭ ��

	if (pokemonId < 1 || pokemonId >(int)frameCounts.size())
		throw std::out_of_range("Invalid Pokemon ID");

	int index = pokemonId - 1;

	// ��ǥ ���� ����
	int currentX = 0;  // 0~29
	int currentY = 0;  // �� ��ȣ
	int framesBefore = 0;

	// ID �������� ������ �����ϸ� ��ǥ ��ġ ã��
	for (int i = 0; i < index; ++i)
	{
		int frames = frameCounts[i];
		if (currentX + frames > sheetCols) {
			// ���θ� �Ѿ�� �� �ٲ�
			currentX = 0;
			currentY += 2; // ���δ� �׻� 2�� ����
		}
		currentX += frames;
	}

	// ���� ���ϸ� ���� ��ǥ
	if (currentX + frameCounts[index] > sheetCols) {
		currentX = 0;
		currentY += 2;
	}
	std::cout << "���� Y : " << std::to_string(currentX) << std::endl;
	int startX = (currentX * cellSize) + (currentX + 1);
	//int startY = (currentY * cellSize) + ((currentY - 1 < 0 ? 1 : currentY - 1) * topOffset) + (currentY * 0.5);
	int startY = (currentY * cellSize) + (((currentY * 0.5) + 1) * topOffset) + (currentY * 0.5);
	//std::cout << "Y : " << std::to_string(startY) << std::endl;
	// ������ Rects
	std::vector<sf::IntRect> rects;
	int frameCount = frameCounts[index];
	for (int i = 0; i < frameCount; ++i) {
		int localX = (currentX + i) % sheetCols;
		int localRow = (i / sheetCols); // �ʿ���� ���� ����
		rects.emplace_back(startX + (i * cellSize), startY, cellSize, cellSize);
	}

	return rects;
}

void SceneBattle::CreateMoveAndPokemon()
{
	//PokemonDB::Instance().LoadFromJson("data/_pokemon_001-151.json");
	//PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");
	//MoveDB::Instance().LoadFromJson("data/moves_korean_kr_full.json");
	Pokemon* ePtr = PokemonDB::Instance().GetPokemon(Utils::RandomRange(1, 152));
	if(isRed) ePtr = PokemonDB::Instance().GetRedPokemon();
	Pokemon* mPtr = PokemonDB::Instance().GetMyPokemon();
	if (!ePtr) {
		std::cerr << "�� ���ϸ��� �����ϴ�!" << std::endl;
		return;
	}
	if (!mPtr) {
		std::cerr << "�� ���ϸ��� �����ϴ�!" << std::endl;
		return;
	}
	ePoke = *ePtr;
	mPoke = *mPtr;
	if (!isRed) ePoke.level = mPoke.level;
	ePoke.ApplyLevelScaling();
	mPoke.ApplyLevelScaling();
	eHp = ePoke.hp;
	mHp = mPoke.hp;

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
			ePoke.moves.push_back(m);
		} else mPoke.moves.push_back(m);
	}
	bmgr.Init(reinterpret_cast<PlayerPokemon*>(&mPoke),
		reinterpret_cast<EnemyPokemon*>(&ePoke));
}


void SceneBattle::Exit()
{
	Scene::Exit();
	uiMgrBattle.Clear();       

	ePoke = {};
	mPoke = {};
	eHp = mHp = 0;
	isRed = false;
}

void SceneBattle::BuildUI()
{
	CreateMoveAndPokemon();
	tex.loadFromFile("graphics/battelBackGround.png");
	sprite.setTexture(tex);
	sprite.setScale(4.3f, 7.f);

	pokeName1 = new Text("pokename1");
	sf::Vector2f pos1 = { 400.f, FRAMEWORK.GetWindowSize().y / 2.f - 250.f };

	pokeName1->SetBackGround("graphics/battle_Sprite.png", { 0, 0, 112, 35 }, 5.f, 5.f);
	pokeName1->SetFillColor(sf::Color::Black);
	pokeName1->AddText("fonts/pokemon-dppt.otf", ePoke.name, 40, ScreenToUi((sf::Vector2i)pos1));
	pokeName1->GetText().setPosition({ pokeName1->GetPosition().x - 190.f, pokeName1->GetPosition().y - 40.f });
	uiMgrBattle.Add(pokeName1);

	pokeLv1 = new Text("pokeLv1");
	pokeLv1->SetFillColor(sf::Color::Black);
	pokeLv1->AddText("fonts/pokemon-dppt.otf", std::to_string(ePoke.level), 50, ScreenToUi((sf::Vector2i)(pos1 + sf::Vector2f(455.f, -7.f))));
	pokeLv1->GetText().setPosition({ pokeLv1->GetPosition().x - 200.f, pokeLv1->GetPosition().y - 40.f });
	uiMgrBattle.Add(pokeLv1);

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
	pokeName2->AddText("fonts/pokemon-dppt.otf", mPoke.name, 40, ScreenToUi((sf::Vector2i)pos2));
	pokeName2->GetText().setPosition({ pokeName2->GetPosition().x - 135.f, pokeName2->GetPosition().y - 60.f });
	uiMgrBattle.Add(pokeName2);

	pokeLv2 = new Text("pokeLv2");
	pokeLv2->SetFillColor(sf::Color::Black);
	pokeLv2->AddText("fonts/pokemon-dppt.otf", std::to_string(mPoke.level), 50, ScreenToUi((sf::Vector2i)(pos2 + sf::Vector2f(515.f, -17.f))));
	pokeLv2->GetText().setPosition({ pokeLv2->GetPosition().x - 200.f, pokeLv2->GetPosition().y - 40.f });
	uiMgrBattle.Add(pokeLv2);

	ExpBar.setSize({ 320.f, 10.f });
	ExpBar.setFillColor(sf::Color(148, 248, 148));
	ExpBar.setPosition(ScreenToUi((sf::Vector2i)(pos2 + sf::Vector2f(-134.f, 56.f))));

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
	std::wstring ts = L"������ �ұ�?";
	if (!isBattleNpcOrPos) ts += L" [E : ���ͺ�]";
	battleMsg->AddText("fonts/pokemon-dppt.otf", ts, 40, ScreenToUi((sf::Vector2i)pos3));
	battleMsg->GetText().setPosition({ 10.f, battleMsg->GetPosition().y - 60.f });
	uiMgrBattle.Add(battleMsg);


	mov1 = new Button("mov1");
	mov1->isMouseOverColor = true;
	mov1->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo1Id = Utils::RandomRange(1, 100);
	mov1->AddButton(sf::String(mPoke.moves[0].name) + "\n" + std::to_string(mPoke.moves[0].pp), 40, sf::Color::Black);
	sf::Vector2f mpos1 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov1->SetPosition(ScreenToUi((sf::Vector2i)mpos1));
	mov1->SetOutlineColor(sf::Color::Black);
	mov1->SetOutlineThickness(4.f);
	mov1->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos1.x - 110.f, mpos1.y - 15.f)));
	mov1->SetOnClick([=]() { UseMoveNum(0, mov1); });
	uiMgrBattle.Add(mov1);

	mov2 = new Button("mov2");
	mov2->isMouseOverColor = true;
	mov2->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo2Id = Utils::RandomRange(1, 100);
	mov2->AddButton(sf::String(mPoke.moves[1].name) + "\n" + std::to_string(mPoke.moves[1].pp), 40, sf::Color::Black);
	sf::Vector2f mpos2 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 125.f };
	mov2->SetPosition(ScreenToUi((sf::Vector2i)mpos2));
	mov2->SetOutlineColor(sf::Color::Black);
	mov2->SetOutlineThickness(4.f);
	mov2->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos2.x - 110.f, mpos2.y - 15.f)));
	mov2->SetOnClick([=]() { UseMoveNum(1, mov2); });
	uiMgrBattle.Add(mov2);

	mov3 = new Button("mov3");
	mov3->isMouseOverColor = true;
	mov3->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo3Id = Utils::RandomRange(1, 100);
	mov3->AddButton(sf::String(mPoke.moves[2].name) + "\n" + std::to_string(mPoke.moves[2].pp), 40, sf::Color::Black);
	sf::Vector2f mpos3 = { FRAMEWORK.GetWindowSize().x / 2.f + 250.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov3->SetPosition(ScreenToUi((sf::Vector2i)mpos3));
	mov3->SetOutlineColor(sf::Color::Black);
	mov3->SetOutlineThickness(4.f);
	mov3->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos3.x - 110.f, mpos3.y - 15.f)));
	mov3->SetOnClick([=]() { UseMoveNum(2, mov3); });
	uiMgrBattle.Add(mov3);

	mov4 = new Button("mov4");
	mov4->isMouseOverColor = true;
	mov4->SetButton({ 200.f, 100.f }, sf::Color::White, "fonts/pokemon-dppt.otf");
	int mo4Id = Utils::RandomRange(1, 100);
	mov4->AddButton(sf::String(mPoke.moves[3].name) + "\n" + std::to_string(mPoke.moves[3].pp), 40, sf::Color::Black);
	sf::Vector2f mpos4 = { FRAMEWORK.GetWindowSize().x / 2.f + 300.f + 210.f, FRAMEWORK.GetWindowSize().y - 50.f };
	mov4->SetPosition(ScreenToUi((sf::Vector2i)mpos4));
	mov4->SetOutlineColor(sf::Color::Black);
	mov4->SetOutlineThickness(4.f);
	mov4->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(mpos4.x - 110.f, mpos4.y - 15.f)));
	mov4->SetOnClick([=]() { UseMoveNum(3, mov4); });
	uiMgrBattle.Add(mov4);
	uiMgrBattle.Init();


	int getI = ePoke.id;
	std::cout << "��ȣ ��� : " << std::to_string(pokemonFrameCounts.size()) << std::endl;
	std::vector<sf::IntRect> rects = {
		/*{0, 35, 80, 80},
		{80, 35, 80, 80}*/
		GetPokemonRects(getI, pokemonFrameCounts)[0],
		GetPokemonRects(getI, pokemonFrameCounts)[1]
	};

	pokTex1 = Utils::loadWithColorKey("graphics/pokemon_list.png", sf::Color(147, 187, 236), sf::Color(50, 97, 168));
	pokSprite1.setTexture(pokTex1);
	pokSprite1.setPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(740.f, 120.f)));
	pokSprite1.setScale(5.f, 5.f);
	poke1 = new SpriteAnimator(&pokSprite1, rects, 0.35f);

	pokTex2 = Utils::loadWithColorKey("graphics/pokemon_list.png", sf::Color(147, 187, 236), sf::Color(50, 97, 168));
	pokSprite2.setTexture(pokTex2);
	pokSprite2.setTextureRect(GetPokemonRects(mPoke.id, pokemonFrameCounts)[2]);
	pokSprite2.setPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(150.f, 310.f)));
	pokSprite2.setScale(5.f, 5.f);
	//bmgr.SetSprite(&pokSprite1, &pokSprite2);
}

void SceneBattle::UseMoveNum(int n, Button* b)
{
	if (static_cast<int>(bmgr.GetCurrentTurn()) != 0) return;
	mPoke.moves[n].pp--;
	if (mPoke.moves[n].pp < 0) mPoke.moves[n].pp = 0;
	if (mPoke.moves[n].pp > 0) bmgr.UseMove(mPoke.moves[n].id);
	else battleMsg->SetString(L"�ش� ����� ���̻� PP�� ����!");
	b->SetString(sf::String(mPoke.moves[n].name) + "\n" + std::to_string(mPoke.moves[n].pp));
}

void SceneBattle::Init()
{
	Scene::Init();
	//BuildUI();
}

void SceneBattle::Enter()
{
	Scene::Enter();
	PokemonDB::Instance().MyPokeClear();
	PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");
	BuildUI();
	//CreateMoveAndPokemon();
}

void SceneBattle::Update(float dt)
{
	Scene::Update(dt);
	uiMgrBattle.Update(dt);
	bmgr.Update(dt);
	poke1->Update(dt, true);
	if(bmgr.BattleOver() && !isEnding) {
		overTime += dt;
		if (overTime > 2.f) {
			pmgr.LoadGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
			ar = pmgr.GetAll();
			ar[mPoke.id].hp = mPoke.hp;
			ar[mPoke.id].level = mPoke.level;
			ar[mPoke.id].experience = mPoke.experience;
			pmgr.AddPokemon(ar[mPoke.id]);
			pmgr.SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");

			if (isRed) {
				battleMsg->SetString(L"...... �׷���.. \n�ʰ� ���� è�Ǿ��̱�.");
				overTime = 0.f;
				isEnding = true;
			}
			else {
				SCENE_MGR.ChangeScene(SceneIds::Game);
				overTime = 0.f;
			}
		}
	}

	if (isEnding) {
		overTime += dt;
		if (overTime > 2.f) {
			if (!bgmMusic->openFromFile("sounds/Ending.wav")) // ���� ���� ���
			{
				std::cout << "���� ���� ���� ����!" << std::endl;
			}
			bgmMusic->play();
			SCENE_MGR.ChangeScene(SceneIds::End);
			isEnding = false;
			overTime = 0.f;
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::E) && !isBattleNpcOrPos) {
		bmgr.getAddPokemon(ePoke, playerInv);
	}

	float maxHpWidth = 242.f;
	float maxExpBar = 320.f;


	int ecurrentHp = ePoke.hp;
	int mcurrentHp = mPoke.hp;

	int mcurrentExp = mPoke.experience;

	float ehpRatio = static_cast<float>(ecurrentHp) / eHp;
	if (ehpRatio < 0.f) ehpRatio = 0.f;
	if (ehpRatio > 1.f) ehpRatio = 1.f;

	float mhpRatio = static_cast<float>(mcurrentHp) / mHp;
	if (mhpRatio < 0.f) mhpRatio = 0.f;
	if (mhpRatio > 1.f) mhpRatio = 1.f;

	float mExpRatio = static_cast<float>(mcurrentExp) / 100.f;
	if (mExpRatio < 0.f) mExpRatio = 0.f;
	if (mExpRatio > 1.f) mExpRatio = 1.f;

	pokeHp1.setSize({ maxHpWidth * ehpRatio, 20.f });
	pokeHp2.setSize({ maxHpWidth * mhpRatio, 20.f });
	ExpBar.setSize({ maxExpBar * mExpRatio, 10.f });
	pokeLv2->SetString(std::to_string(mPoke.level));
}

void SceneBattle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(sprite);
	window.draw(pokSprite2);
	uiMgrBattle.Draw(window);
	window.draw(pokSprite1);
	window.draw(pokeBackHp1);
	window.draw(pokeHp1);
	window.draw(pokeBackHp2);
	window.draw(pokeHp2);
	window.draw(ExpBar);
}