#pragma once
#include "Scene.h"

class Button;
class ScenePokeDev : public Scene
{
protected:
	Button* button = nullptr;
public:
	ScenePokeDev();
	~ScenePokeDev() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

