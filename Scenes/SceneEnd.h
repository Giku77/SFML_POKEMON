#pragma once
#include "Scene.h"

class SpriteAnimator;
class Text;

class SceneEnd : public Scene
{
protected:
	SpriteAnimator* end = nullptr;
	Text* endText = nullptr;
	//UiMgr EndingUI;

	sf::Sprite newScreen;
	sf::Texture newTex;


public:
	SceneEnd();
	~SceneEnd() override;

	void Init() override;
	void Enter() override;
	void Draw(sf::RenderWindow& window) override;

	void Update(float dt) override;
};

