#pragma once
#include "Scene.h"

class TileMapGameObject;
class AniPlayer;
class SpriteAnimator;
class ShopUI;

class SceneGame : public Scene
{
protected:
	TileMapGameObject* tileMapObj = nullptr;
	AniPlayer* player = nullptr;
	SpriteAnimator* ani = nullptr;
	ShopUI* shopUi = nullptr;

	int playerGold = 3000;


	sf::Sprite newScreen;
	sf::Texture newTex;

	bool isCenterEnter = false;
	bool isShopEnter = false;
	bool isBattle = false;
	bool shopOpened = false;
	float aniCenterTime = 0.f;

public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

