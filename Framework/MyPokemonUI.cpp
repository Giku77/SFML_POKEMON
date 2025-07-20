#include "stdafx.h"
#include "MyPokemonUI.h"
#include "PokemonDB.h"

MyPokemonUI::MyPokemonUI(const std::string& name)
	:UI(name)
{
}

MyPokemonUI::~MyPokemonUI()
{
	
}

void MyPokemonUI::DataReload()
{
	index = 0;
	PokemonDB::Instance().MyPokeClear();
	PokemonDB::Instance().LoadFromPlayerJson("data/player_pokemon.json");
	//PokemonDB::Instance().LoadFromJson("data/_pokemon_001-151.json");
	Mypokemons = &PokemonDB::Instance().GetMyPokemonsRef();
}

void MyPokemonUI::Init()
{
	DataReload();

	int i = 0;
	for (auto& p : *Mypokemons) {
		if (Mypokemons->size() - 1 < i) break;
		PokeListUi* pokeUi = new PokeListUi();
		pokeUi->pokBg = Utils::loadWithColorKey("graphics/MyPokemonList.png", sf::Color(255, 255, 255));
		pokeUi->pokBgSpr.setTexture(pokeUi->pokBg);
		pokeUi->pokBgSpr.setTextureRect({ 377, 57, 136, 54 });
		pokeUi->pokBgSpr.setScale(2.f, 2.f);
		pokeUi->pokBgSpr.setPosition(0.f, 0.f + (100.f * i));
		
		pokeUi->aHp.setSize({ 98.f, 9.f });
		pokeUi->aHp.setFillColor(sf::Color(60, 60, 60));
		pokeUi->aHp.setPosition(pokeUi->pokBgSpr.getPosition().x + 139.f, pokeUi->pokBgSpr.getPosition().y + 60.f);

		pokeUi->nHp.setSize({ 98.f, 9.f });
		pokeUi->nHp.setFillColor(sf::Color(180, 60, 60));
		pokeUi->nHp.setPosition(pokeUi->pokBgSpr.getPosition().x + 139.f, pokeUi->pokBgSpr.getPosition().y + 60.f);

		pokeUi->name = new Text("name");
		pokeUi->name->SetFillColor(sf::Color::Black);
		pokeUi->name->AddText("fonts/pokemon-dppt.otf", p.second.name, 30, sf::Vector2f(110.f, 25.f));
		uiMgrMyPoke.Add(pokeUi->name);

		pokeUi->level = new Text("lv");
		pokeUi->level->SetFillColor(sf::Color::Black);
		pokeUi->level->AddText("fonts/pokemon-dppt.otf", std::to_string(p.second.level), 30, sf::Vector2f(65.f, 70.f));
		uiMgrMyPoke.Add(pokeUi->level);

		pokeUi->hp = new Text("hp");
		pokeUi->hp->SetFillColor(sf::Color::Black);
		pokeUi->hp->AddText("fonts/pokemon-dppt.otf", "HP   " + std::to_string(p.second.hp), 30, sf::Vector2f(190.f, 70.f));
		uiMgrMyPoke.Add(pokeUi->hp);

		pok.push_back(pokeUi);
		i++;
	}
	uiMgrMyPoke.Init();
	std::cout << "호출" << std::endl;
}

void MyPokemonUI::Release()
{
}

void MyPokemonUI::Reset()
{
}

void MyPokemonUI::Update(float dt)
{
	if (!GetActive()) return;
	uiMgrMyPoke.Update(dt);
	int idx = 0;
	const auto& list = PokemonDB::Instance().GetMyPokemonsRef();  
	for (const auto& kv : list) {    
		int id = kv.first;    
		const Pokemon& live = kv.second;   

		// 최대 HP는 종(도감) DB에서 얻음
		int maxHp = PokemonDB::Instance().GetPokemon(id)->hp;

		float ratio = Utils::Clamp(static_cast<float>(live.hp) / maxHp, 0.f, 1.f);

		pok[idx]->nHp.setSize({ 98.f * ratio, 9.f });
		pok[idx]->hp->SetString("HP   " + std::to_string(live.hp));

		pok[idx]->level->SetString(std::to_string(live.level));

		pok[idx]->name->SetString(PokemonDB::Instance().GetPokemon(id)->name);

		++idx;
		if (idx >= pok.size()) break;     
	}
}

void MyPokemonUI::Draw(sf::RenderWindow& window)
{
	if (!GetActive()) return;
	for (auto& p : pok) {
		window.draw(p->pokBgSpr);
		window.draw(p->aHp);
		window.draw(p->nHp);
	}
	uiMgrMyPoke.Draw(window);
}
