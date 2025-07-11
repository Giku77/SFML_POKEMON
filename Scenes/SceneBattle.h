#pragma once
#include "Scene.h"
class SceneBattle : public Scene
{
protected:

public:
	SceneBattle();
	~SceneBattle() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

