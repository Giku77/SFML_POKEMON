#pragma once

class PokemonDB
{
public:
    static PokemonDB& Instance()
    {
        static PokemonDB inst;
        return inst;
    }

    bool LoadFromJson(const std::string& jsonPath)
    {
        std::ifstream in(jsonPath);
        if (!in.is_open()) return false;
        nlohmann::json j; in >> j;


        const auto& jPokemons = j["pokemons"];
        for (const auto& obj : jPokemons)
        {
            Pokemon d;
            d.id = obj["id"].get<int>();
            d.name = obj["name"].get<std::wstring>();
            d.level = obj["level"].get<int>();
            d.experience = obj["experience"].get<int>();
            d.hp = obj["hp"].get<int>();
            d.attack = obj["attack"].get<int>();
            d.defense = obj["defense"].get<int>();
            Move m = { 10, L"ÇÒÄû±â", 40, 100, L"³ë¸»" };
            d.moves.push_back(&m);
            pokemons[d.id] = std::move(d);
        }

        return true;
    }

    bool LoadFromPlayerJson(const std::string& jsonPath)
    {
        std::ifstream in(jsonPath);
        if (!in.is_open()) return false;
        nlohmann::json j; in >> j;


        const auto& jPokemons = j["pokemons"];
        for (const auto& obj : jPokemons)
        {
            Pokemon d;
            d.id = obj["id"].get<int>();
            d.name = obj["name"].get<std::wstring>();
            d.level = obj["level"].get<int>();
            d.experience = obj["experience"].get<int>();
            d.hp = obj["hp"].get<int>();
            d.attack = obj["attack"].get<int>();
            d.defense = obj["defense"].get<int>();
            Move m = { 10, L"ÇÒÄû±â", 40, 100, L"³ë¸»" };
            d.moves.push_back(&m);
            Mypokemons[d.id] = std::move(d);
        }

        return true;
    }


    Pokemon* GetPokemon(int id) 
    {
        auto f = pokemons.find(id);
        return (f == pokemons.end()) ? nullptr : &f->second;
    }

    Pokemon* GetMyPokemon()
    {
        if (!Mypokemons.empty()) {
            auto it = Mypokemons.begin();
            int key = it->first;
            Pokemon value = it->second;
            return &value;
        }
        return nullptr;
    }

private:
    std::unordered_map<int, Pokemon> pokemons;
    std::unordered_map<int, Pokemon> Mypokemons;

    PokemonDB() = default;
    ~PokemonDB() = default;
    PokemonDB(const PokemonDB&) = delete;
    PokemonDB& operator=(const PokemonDB&) = delete;
};




