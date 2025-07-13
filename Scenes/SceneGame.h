#pragma once
#include "Scene.h"

class TileMapGameObject;
class AniPlayer;
class SceneGame : public Scene
{
protected:
	TileMapGameObject* tileMapObj;
	AniPlayer* player = nullptr;
public:
	SceneGame();
	~SceneGame() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

