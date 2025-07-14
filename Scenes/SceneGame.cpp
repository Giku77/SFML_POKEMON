#include "stdafx.h"
#include "SceneGame.h"
#include "AniPlayer.h"
#include "TileMapGameObject.h"


SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	ANI_CLIP_MGR.Load("animations/player_idle.csv");
	ANI_CLIP_MGR.Load("animations/player_run.csv");
	ANI_CLIP_MGR.Load("animations/player_Up.csv");
	ANI_CLIP_MGR.Load("animations/player_Down.csv");


	texIds.push_back("graphics/player.png");

	tileMapObj = (TileMapGameObject*)AddGameObject(new TileMapGameObject("TileMap"));

	player = (AniPlayer*)AddGameObject(new AniPlayer("Player"));
	std::cout << player->GetPosition().x << " / " << player->GetPosition().y << std::endl;
	/*if (!tileMap.load("data/world.tmj"))
		std::cerr << "맵 로드 실패!\n";*/

	//worldView.setCenter(500.f, 1100.f);
	//worldView.setCenter(tileMap.GetMapPixelSize() * 0.5f);
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
	Scene::Update(dt);
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

		if (tileMapObj->isCollidable(tileX, tileY)) {
			//std::cout << "충돌" << std::endl;
			playerPos = player->getPrevPos();
		}
		player->SetPosition(playerPos);

		playerPos.x = Utils::Clamp(playerPos.x, halfSize.x, mapSize.x - halfSize.x);
		playerPos.y = Utils::Clamp(playerPos.y, halfSize.y, mapSize.y - halfSize.y);


		worldView.setCenter(playerPos);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	//window.setView(worldView);
	//tileMap.draw(window);  

	Scene::Draw(window);
}
