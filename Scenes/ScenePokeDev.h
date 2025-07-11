#pragma once
#include "Scene.h"

class Button;
class Text;
class ScenePokeDev : public Scene
{
protected:
	Button* button = nullptr;
	Text* text = nullptr;
public:
	ScenePokeDev();
	~ScenePokeDev() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

