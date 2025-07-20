#include "stdafx.h"
#include "SceneGame.h"
#include "AniPlayer.h"
#include "TileMapGameObject.h"
#include "SpriteAnimator.h"
#include "ShopUI.h"
#include "MyPokemonUI.h"
#include "InventoryUI.h"
#include "PokemonDB.h"
#include "SceneBattle.h"


SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

SceneGame::~SceneGame()
{
	delete ani;
	ani = nullptr;
	delete shopUi;
	shopUi = nullptr;
	delete mypokeUi;
	mypokeUi = nullptr;
	uMgr.Clear();
}

void SceneGame::Init()
{
	soundIds.push_back("sounds/UiSound.wav");
	//PokemonDB::Instance().LoadFromJson("data/_pokemon_001-151.json");
	ANI_CLIP_MGR.Load("animations/player_idle.csv");
	ANI_CLIP_MGR.Load("animations/player_idle_Up.csv");
	ANI_CLIP_MGR.Load("animations/player_idle_Right.csv");
	ANI_CLIP_MGR.Load("animations/player_idle_Left.csv");
	ANI_CLIP_MGR.Load("animations/player_run.csv");
	ANI_CLIP_MGR.Load("animations/player_Up.csv");
	ANI_CLIP_MGR.Load("animations/player_Down.csv");


	texIds.push_back("graphics/player.png");

	fontIds.push_back("fonts/pokemon-dppt.otf");
	tileMapObj = (TileMapGameObject*)AddGameObject(new TileMapGameObject("TileMap"));

	player = (AniPlayer*)AddGameObject(new AniPlayer("Player"));

	/*if (!tileMap.load("data/world.tmj"))
		std::cerr << "맵 로드 실패!\n";*/
	
	//worldView.setCenter(500.f, 1100.f);
	//worldView.setCenter(tileMap.GetMapPixelSize() * 0.5f);

	newTex.loadFromFile("graphics/EnterPokeCenter.png");
	newScreen.setTexture(newTex);
	sf::Vector2f pos = { 0.f,  0.f };
	newScreen.setPosition(ScreenToUi((sf::Vector2i)pos));
	newScreen.setScale(0.8f, 0.8f);

	ani = new SpriteAnimator(&newScreen, newTex, 540.f, 411.f, 0.03f);

	shopUi = new ShopUI("MartPanel");
	shopUi->Init();
	shopUi->SetActive(false);

	mypokeUi = new MyPokemonUI("MyPokemonList");
	mypokeUi->Init();
	mypokeUi->SetActive(false);

	invUI = new InventoryUI("Inv");
	invUI->Init();
	invUI->SetActive(false);

	MsgMgr.Load("data/messages.json");

	button = new Button("MsgButton", true);
	button->SetBackGroud("graphics/18507.png", 4.f, 4.f);
	button->SetOnClick([=]() mutable {
		std::wstring next = storyGame.Next();
		if (next.empty()) {
			isMsgbox = false;
			if (isBattle) {
				isBattle = false;

				if (!wipe && !isRed) wipe.reset(new SlashWipe(FRAMEWORK.GetWindowSize(), uiView));
				if (!trans && isRed) trans.reset(new BattleTransition(FRAMEWORK.GetWindowSize(), 0, uiView));
				//SCENE_MGR.ChangeScene(SceneIds::Battle);
			}
			return;
		}
		//std::cout << "클릭 인덱스 : " << storyGame.getIndex() << std::endl;
		button->SetString(next);
	});
	button->SetButton({ FRAMEWORK.GetWindowSize().x / 2.f + 200.f, 300.f }, sf::Color::Black, "fonts/pokemon-dppt.otf");
	button->AddButton("", 40, sf::Color::Black);
	sf::Vector2f Msgpos = { 825.f, 430.f };
	button->SetPosition(ScreenToUi((sf::Vector2i)Msgpos));
	button->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(250.f, Msgpos.y - 50.f)));
	uMgr.Add(button);
	uMgr.Init();

	Scene::Init();
}

void SceneGame::Enter()
{
	std::cout << "게임씬 입장 : " << lastPlayerPos.x << std::endl;
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	player->SetPosition(lastPlayerPos);
	worldView.setCenter({ lastPlayerPos.x, lastPlayerPos.y + 100.f });
	player->setPrevPos(lastPlayerPos);
	if (!mainBgm.openFromFile("sounds/mainGame.wav")) // 음악 파일 경로
	{
		std::cout << "음악 파일 열기 실패!" << std::endl;
	}

	mainBgm.setLoop(true);   // 루프 재생 여부 (배경음이라면 보통 true)
	mainBgm.setVolume(100.f); // 볼륨 (0~100)
	mainBgm.play();
	//wipe.reset();
	Scene::Enter();
}

void SceneGame::Exit()
{
	if (player) {
		std::cout << "게임씬 이탈" << std::endl;
		lastPlayerPos = player->GetPosition();
	}
	isRed = false;
	//uMgr.Clear();
	wipe.reset();
	mainBgm.stop();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{

	if (wipe && !isRed)
	{
		if (wipe->update(dt)) {
			SCENE_MGR.ChangeScene(SceneIds::Battle);
			return;
		}
	}

	if (trans && isRed)
	{
		if (trans->update(dt)) {
			trans.reset();
			SCENE_MGR.ChangeScene(SceneIds::Battle);
			return;
		}
	}

	//std::cout << "애니메이션 :" << player->getAnimator().GetCurrentClipId() << std::endl;
	mypokeUi->Update(dt);
	invUI->Update(dt);
	Scene::Update(dt);
	ani->Update(dt, true);
	uMgr.Update(dt);

	if (isCenterEnter) aniCenterTime += dt;
	if (aniCenterTime > 4.f) {
		aniCenterTime = 0.f;
		//PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");

		Pmgr.LoadGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");

		//MyPokemons = PokemonDB::Instance().GetMyPokemons();
		MyPokemons = Pmgr.GetAll();
		std::cout << "회복 사이즈 : " << MyPokemons.size() << std::endl;
		for (auto& p : MyPokemons) {
			p.second.hp = PokemonDB::Instance().GetPokemon(p.second.id)->hp;
			Pmgr.AddPokemon(p.second);
			//PokemonDB::Instance().AddMyPokemon(p.second);
		}
		Pmgr.SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
		//PokemonDB::Instance().SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
		isCenterEnter = false;
	}
	if (shopOpened)
	{
		if (shopUi->IsOpen())         
		{
			shopUi->Update(dt);

			if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
				shopUi->Close();
			return;                   
		}
		else                            
		{
			shopOpened = false;         
		}
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::P)) {
		SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("sounds/UiSound.wav"),false);
		if(mypokeUi->GetActive()) mypokeUi->SetActive(false);
		else {
			mypokeUi->DataReload();
			mypokeUi->SetActive(true);
		}
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("sounds/UiSound.wav"),false);
		if (invUI->GetActive()) invUI->Close();
		else {
			invUI->SetActive(true);
			playerInv.LoadFromJson("data/player_inventory.json");
			invUI->Open(playerInv);
		}
	}


	//player->SetScale({ 0.5f, 0.5f });
	/*const float moveSpeed = 400.f;

	sf::Vector2f moveDir(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  moveDir.x -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) moveDir.x += 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    moveDir.y -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  moveDir.y += 1.f;

	worldView.move(moveDir * moveSpeed * dt);*/

	if (player != nullptr)
	{
		sf::Vector2f playerPos = player->GetPosition();

		// 경계 제한
		sf::Vector2f viewSize = worldView.getSize();
		sf::Vector2f halfSize = viewSize * 0.5f;
		sf::Vector2f mapSize = tileMapObj->GetMapSize();

		playerPos.x = Utils::Clamp(playerPos.x, 0.f, mapSize.x);
		playerPos.y = Utils::Clamp(playerPos.y, 0.f, mapSize.y);

		int tileX = static_cast<int>(playerPos.x) / tileMapObj->getTileW();
		int tileY = static_cast<int>(playerPos.y) / tileMapObj->getTileH();

	
		if (tileMapObj->isCenterEnterable(tileX, tileY)) {
			//std::cout << "입장" << std::endl;
			ani->setIndex(0);
			isCenterEnter = true;
		}

		if (tileMapObj->isShopEnterable(tileX, tileY)) {
			//std::cout << "입장" << std::endl;
			playerInv.LoadFromJson("data/player_inventory.json");
			shopUi->Open("poke_mart", playerGold, playerInv);
			shopOpened = true;
			shopUi->SetActive(true);
			//ani->setIndex(0);
			isShopEnter = true;
		}

		if (tileMapObj->isBattleable(tileX, tileY)) {
			//std::cout << "배틀" << std::endl;
			isBattle = true;
			auto* battle = dynamic_cast<SceneBattle*>(
				SCENE_MGR.Get(SceneIds::Battle));
			if (battle) battle->SetisBattleNpcOrPos(true);
		}

		//battlePosTime += dt;
		bool stepped = (tileX != lastStepX || tileY != lastStepY);
		if (stepped)
		{
			lastStepX = tileX;
			lastStepY = tileY;

			if (tileMapObj->isPosBattleable(tileX, tileY))
			{
				
				const float baseRate = 0.041667f;         
				float rate = baseRate;                    

				if (Utils::RandomRange(0.f, 1.f) < rate)   
				{
					isBattle = true;
					isMsgbox = true;
					auto* battle = dynamic_cast<SceneBattle*>(
						SCENE_MGR.Get(SceneIds::Battle));
					if (battle) battle->SetisBattleNpcOrPos(false);
					storyGame.Load("battleZ", MsgMgr);
					button->SetString(storyGame.GetCurrent());
					storyGame.Next();
				}
			}
		}

		std::string id = tileMapObj->getNpcId(tileX, tileY);
		if (!id.empty() && InputMgr::GetKeyDown(sf::Keyboard::LShift)) {
			std::cout << "NPC 대화 여부" << std::endl;
			if (id == "RED") {
				//std::cout << "NPC 대화 레드" << std::endl;
				auto* battle = dynamic_cast<SceneBattle*>(
					SCENE_MGR.Get(SceneIds::Battle));
				battle->SetisRed(true);
				isRed = true;
			}
			isMsgbox = true;
			storyGame.Load(id, MsgMgr);
			button->SetString(storyGame.GetCurrent());
			storyGame.Next();
		}

		if (tileMapObj->isCollidable(tileX, tileY)) {
			//std::cout << "충돌" << std::endl;
			playerPos = player->getPrevPos();
			//player->SetPosition(playerPos);
		}

		//tileX = static_cast<int>(playerPos.x) / tileMapObj->getTileW();
		//tileY = static_cast<int>(playerPos.y) / tileMapObj->getTileH();
		//std::cout << "x : " << tileX << "y : " << tileY << std::endl;

		playerPos.x = Utils::Clamp(playerPos.x, 0.f, mapSize.x);
		playerPos.y = Utils::Clamp(playerPos.y, 0.f, mapSize.y);
		player->SetPosition(playerPos);

		sf::Vector2f camCenter = playerPos;
		camCenter.x = Utils::Clamp(camCenter.x, halfSize.x, mapSize.x - halfSize.x);
		camCenter.y = Utils::Clamp(camCenter.y, halfSize.y, mapSize.y - halfSize.y);
		worldView.setCenter(camCenter);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(window.getDefaultView());
	if(isCenterEnter) window.draw(newScreen);
	if(shopOpened) shopUi->Draw(window);
	mypokeUi->Draw(window);
	invUI->Draw(window);
	if(isMsgbox) uMgr.Draw(window);
	if(trans) trans->draw(window);
	if(cover) cover->draw(window);
	if(wipe) wipe->draw(window);
	//window.draw(newScreen);
}
