#pragma once
#include "Scene.h"
#include "Inventory.h"
#include "StoryPlayer.h"
#include "BattleTransition.cpp"

class TileMapGameObject;
class AniPlayer;
class SpriteAnimator;
class ShopUI;
class MyPokemonUI;
class InventoryUI;

class SceneGame : public Scene
{
protected:
	Inventory playerInv;
	InventoryUI* invUI = nullptr;
	Button* button = nullptr;
	TileMapGameObject* tileMapObj = nullptr;
	AniPlayer* player = nullptr;
	SpriteAnimator* ani = nullptr;
	ShopUI* shopUi = nullptr;
	MyPokemonUI* mypokeUi = nullptr;

	std::unordered_map<int, Pokemon> MyPokemons;

	int playerGold = 3000;
	int lastStepX = -1, lastStepY = -1;

	std::unique_ptr<BattleTransition> trans;
	std::unique_ptr<SpinCover> cover;
	std::unique_ptr<SlashWipe > wipe;


	sf::Sprite newScreen;
	sf::Texture newTex;

	bool isCenterEnter = false;
	bool isShopEnter = false;
	bool isBattle = false;
	bool shopOpened = false;
	float aniCenterTime = 0.f;
	float battlePosTime = 0.f;
	PokemonManager Pmgr;
	MessageMgr MsgMgr;
	StoryPlayer storyGame;
	UiMgr uMgr;

	sf::Vector2f lastPlayerPos = { 500.f, 1100.f };

	bool isMsgbox = false;
	bool isRed = false;

public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

