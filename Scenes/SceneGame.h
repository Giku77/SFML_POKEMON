#pragma once
#include "Scene.h"

class TileMapGameObject;
class AniPlayer;
class SpriteAnimator;
class SceneGame : public Scene
{
protected:
	TileMapGameObject* tileMapObj;
	AniPlayer* player = nullptr;
	SpriteAnimator* ani = nullptr;

	sf::Sprite newScreen;
	sf::Texture newTex;

	bool isCenterEnter = false;
	float aniCenterTime = 0.f;

public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

