#pragma once
#include "Scene.h"
class SceneGame : public Scene
{
protected:

public:
	SceneGame();
	~SceneGame() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

