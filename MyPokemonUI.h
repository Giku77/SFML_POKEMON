#pragma once
#include "UI.h"
#include "Text.h"

struct PokeListUi
{
	Text* name = nullptr;
	Text* level = nullptr;
	Text* hp = nullptr;

	sf::Texture pokBg;
	sf::Sprite pokBgSpr;
	sf::Texture pokTex;
	sf::Sprite pokSpr;
	sf::RectangleShape aHp;
	sf::RectangleShape nHp;
};

class MyPokemonUI : public UI
{
private:

	std::vector<PokeListUi*> pok;

	const std::unordered_map<int, Pokemon>* Mypokemons;

	int index = 0;

	UiMgr uiMgrMyPoke;


public:
	MyPokemonUI(const std::string& name = "MyPokemonUI");
	~MyPokemonUI() override;

	void DataReload();

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

