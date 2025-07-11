#pragma once
#include "Scene.h"
class SceneEnd : public Scene
{
protected:

public:
	SceneEnd();
	~SceneEnd() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

