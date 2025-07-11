#pragma once
#include "Scene.h"
class SceneIntro : public Scene
{
protected:

public:
	SceneIntro();
	~SceneIntro() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

