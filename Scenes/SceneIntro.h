#pragma once
#include "Scene.h"
//#include "BattleTransition.cpp"

class Text;
class BattleTransition;
class SceneIntro : public Scene
{
protected:
	Text* StartText = nullptr;
	sf::Texture newTex;
	sf::Sprite newScreen;
	float introTime = 0.f;
	float bgTime = 0.f;

	bool isTextDraw = true;
	bool isBgDraw = true;

	BattleTransition* transs = nullptr;;

public:
	SceneIntro();
	~SceneIntro() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

