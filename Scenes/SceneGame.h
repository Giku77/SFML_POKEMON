#pragma once
#include "Scene.h"

class TileMapGameObject;
class AniPlayer;
class SceneGame : public Scene
{
protected:
	TileMapGameObject* tileMapObj;
	AniPlayer* player = nullptr;
	float prevPostime = 0.f;
	sf::Vector2f prevPos = { 0.f, 0.f };
public:
	SceneGame();
	~SceneGame() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

