#pragma once
#include "Scene.h"

class Button;
class Text;
class SpriteAnimator;
class ScenePokeDev : public Scene
{
protected:
	enum Type {
		BG,
		CS,
	};
	Button* button = nullptr;
	Text* text = nullptr;
	SpriteAnimator* anim = nullptr;

	sf::Sprite backGround;
	sf::Texture bGTex;

	sf::Sprite backGround2;
	sf::Texture bGTex2;

	sf::Sprite balls;
	sf::Texture ballTex;

	bool isNameInputActive = false;
	bool isChoosePokemon = false;

	sf::String RandPokemon;
public:
	ScenePokeDev();
	~ScenePokeDev() override;

	void SetBackground(const std::string& path, const sf::IntRect& rect, sf::Sprite& s, sf::Texture& t, const Type& type, const sf::Vector2f& v = {1.f, 1.f});
	void Init() override;
	void Enter() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

