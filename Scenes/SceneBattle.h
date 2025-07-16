#pragma once
#include "Scene.h"

class UiMgr;
class Text;
class Button;
class SpriteAnimator;

class SceneBattle : public Scene
{
protected:
	sf::Texture tex;
	sf::Sprite sprite;
	UiMgr* uiMgr = nullptr;

	Text* pokeName1 = nullptr;
	Text* pokeName2 = nullptr;
	Text* pokeLv1 = nullptr;
	Text* pokeLv2 = nullptr;

	sf::RectangleShape pokeHp1;
	sf::RectangleShape pokeBackHp1;
	sf::RectangleShape pokeHp2;
	sf::RectangleShape pokeBackHp2;

	Text* battleMsg = nullptr;

	Button* mov1 = nullptr;
	Button* mov2 = nullptr;
	Button* mov3 = nullptr;
	Button* mov4 = nullptr;

	Pokemon* ePoke = nullptr;
	int eHp = 0;
	Pokemon* mPoke = nullptr;
	int mHp = 0;

	sf::Sprite pokSprite1;
	sf::Texture pokTex1;
	SpriteAnimator* poke1 = nullptr;
	sf::Sprite pokSprite2;
	sf::Texture pokTex2;
	SpriteAnimator* poke2 = nullptr;

public:
	SceneBattle();
	~SceneBattle() override;

	void CreateMoveAndPokemon();

	void Init() override;
	void Enter() override;

	//void AddPokemons(const SpriteAnimator* p1, const SpriteAnimator* p2);

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

