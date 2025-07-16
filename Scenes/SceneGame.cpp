#include "stdafx.h"
#include "SceneGame.h"
#include "AniPlayer.h"
#include "TileMapGameObject.h"
#include "SpriteAnimator.h"
#include "ShopUI.h"
#include "MyPokemonUI.h"
#include "InventoryUI.h"


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
	/*if (tileMapObj) {
		delete tileMapObj;
		tileMapObj = nullptr;
	}*/
	//delete player;
	//player = nullptr;
}

void SceneGame::Init()
{
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

	Scene::Init();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	player->SetPosition({ 500.f, 1100.f });
	worldView.setCenter(500.f, 1200.f);
	Scene::Enter();
}

void SceneGame::Update(float dt)
{
	if (isCenterEnter) aniCenterTime += dt;
	if (aniCenterTime > 4.f) {
		aniCenterTime = 0.f;
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
		if(mypokeUi->GetActive()) mypokeUi->SetActive(false);
		else mypokeUi->SetActive(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		if (invUI->GetActive()) invUI->Close();
		else {
			invUI->SetActive(true);
			playerInv.LoadFromJson("data/player_inventory.json");
			invUI->Open(playerInv);
		}
	}

	invUI->Update(dt);
	Scene::Update(dt);
	ani->Update(dt, true);
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
			std::cout << "입장" << std::endl;
			ani->setIndex(0);
			isCenterEnter = true;
		}

		if (tileMapObj->isShopEnterable(tileX, tileY)) {
			std::cout << "입장" << std::endl;
			playerInv.LoadFromJson("data/player_inventory.json");
			shopUi->Open("poke_mart", playerGold, playerInv);
			shopOpened = true;
			shopUi->SetActive(true);
			//ani->setIndex(0);
			isShopEnter = true;
		}

		if (tileMapObj->isBattleable(tileX, tileY)) {
			std::cout << "배틀" << std::endl;
			SCENE_MGR.ChangeScene(SceneIds::Battle);
		}

		if (tileMapObj->isCollidable(tileX, tileY)) {
			//std::cout << "충돌" << std::endl;
			playerPos = player->getPrevPos();
		}

		//tileX = static_cast<int>(playerPos.x) / tileMapObj->getTileW();
		//tileY = static_cast<int>(playerPos.y) / tileMapObj->getTileH();
		//std::cout << "x : " << tileX << "y : " << tileY << std::endl;
		player->SetPosition(playerPos);

		playerPos.x = Utils::Clamp(playerPos.x, halfSize.x, mapSize.x - halfSize.x);
		playerPos.y = Utils::Clamp(playerPos.y, halfSize.y, mapSize.y - halfSize.y);


		worldView.setCenter(playerPos);
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
	//window.draw(newScreen);
}
