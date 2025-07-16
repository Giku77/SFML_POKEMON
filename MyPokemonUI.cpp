#include "stdafx.h"
#include "MyPokemonUI.h"
#include "PokemonDB.h"

UiMgr uiMgr;

MyPokemonUI::MyPokemonUI(const std::string& name)
	:UI(name)
{
}

MyPokemonUI::~MyPokemonUI()
{
}

void MyPokemonUI::Init()
{
	PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");
	pokemons = PokemonDB::Instance().GetMyPokemons();

	for (auto& p :pokemons) {
		int i = 0;
		PokeListUi* pokeUi = new PokeListUi();
		pokeUi->pokBg = Utils::loadWithColorKey("graphics/MyPokemonList.png", sf::Color(255, 255, 255));
		pokeUi->pokBgSpr.setTexture(pokeUi->pokBg);
		pokeUi->pokBgSpr.setTextureRect({ 377, 57, 136, 54 });
		pokeUi->pokBgSpr.setScale(2.f, 2.f);
		pokeUi->pokBgSpr.setPosition(0.f, 0.f + (100.f * i));
		///*pokeUi->aHp.setSize({ 20.f, 5.f });
		//pokeUi->aHp.setFillColor(sf::Color::White);
		//pokeUi->aHp.setPosition(pokeUi->pokBgSpr.getPosition().x + 30.f, pokeUi->pokBgSpr.getPosition().y + 20.f);*/

		pokeUi->name = new Text("name");
		pokeUi->name->SetFillColor(sf::Color::Black);
		pokeUi->name->AddText("fonts/pokemon-dppt.otf", p.second.name, 30, sf::Vector2f(110.f, 25.f));
        uiMgr.Add(pokeUi->name);

		pokeUi->level = new Text("lv");
		pokeUi->level->SetFillColor(sf::Color::Black);
		pokeUi->level->AddText("fonts/pokemon-dppt.otf", std::to_string(p.second.level), 30, sf::Vector2f(65.f, 70.f));
		uiMgr.Add(pokeUi->level);

		pokeUi->hp = new Text("hp");
		pokeUi->hp->SetFillColor(sf::Color::Black);
		pokeUi->hp->AddText("fonts/pokemon-dppt.otf", "HP   " + std::to_string(p.second.hp), 30, sf::Vector2f(190.f, 70.f));
		uiMgr.Add(pokeUi->hp);

		pok.push_back(pokeUi);
		i++;
	}
	uiMgr.Init();

}

void MyPokemonUI::Release()
{
}

void MyPokemonUI::Reset()
{
}

void MyPokemonUI::Update(float dt)
{
	uiMgr.Update(dt);
}

void MyPokemonUI::Draw(sf::RenderWindow& window)
{
	if (!GetActive()) return;
	for (auto& p : pok) {
		window.draw(p->pokBgSpr);
		//window.draw(p->aHp);
	}
	uiMgr.Draw(window);
}
