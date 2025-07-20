#pragma once

using PokePtr = std::shared_ptr<Pokemon>;

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
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string nameStr = obj["name"].get<std::string>();
            d.name = conv.from_bytes(nameStr);
            d.level = obj["level"].get<int>();
            d.experience = obj["experience"].get<int>();
            d.hp = obj["hp"].get<int>();
            d.attack = obj["attack"].get<int>();
            d.defense = obj["defense"].get<int>();
            //Move m = { 10, L"할퀴기", 40, 100, 0, L"노말" };
            //d.moves.push_back(m);
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
            //std::cout << "호출" << std::endl;
            Pokemon d;
            d.id = obj["id"].get<int>();
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string nameStr = obj["name"].get<std::string>();
            d.name = conv.from_bytes(nameStr);
            d.level = obj["level"].get<int>();
            d.experience = obj["experience"].get<int>();
            d.hp = obj["hp"].get<int>();
            d.attack = obj["attack"].get<int>();
            d.defense = obj["defense"].get<int>();
            //Move m = { 10, L"할퀴기", 40, 100, 0, L"노말" };
            //d.moves.push_back(m);
            Mypokemons[d.id] = std::move(d);
        }
        //std::cout << "호출 사이즈 : " << Mypokemons.size() << std::endl;
        return true;
    }

    bool LoadFromRedJson(const std::string& jsonPath)
    {
        std::ifstream in(jsonPath);
        if (!in.is_open()) return false;
        nlohmann::json j; in >> j;


        const auto& jPokemons = j["pokemons"];
        for (const auto& obj : jPokemons)
        {
            //std::cout << "호출" << std::endl;
            Pokemon d;
            d.id = obj["id"].get<int>();
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string nameStr = obj["name"].get<std::string>();
            d.name = conv.from_bytes(nameStr);
            d.level = obj["level"].get<int>();
            d.experience = obj["experience"].get<int>();
            d.hp = obj["hp"].get<int>();
            d.attack = obj["attack"].get<int>();
            d.defense = obj["defense"].get<int>();
            //Move m = { 10, L"할퀴기", 40, 100, 0, L"노말" };
            //d.moves.push_back(m);
            Redpokemons[d.id] = std::move(d);
        }
        //std::cout << "호출 사이즈 : " << Mypokemons.size() << std::endl;
        return true;
    }

    bool SaveGame(const std::wstring& playerName,
        const std::string& filename) const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

        nlohmann::json root;
        root["player"] = conv.to_bytes(playerName);

        nlohmann::json arr;
        for (const auto& p : pokemons) {
            nlohmann::json pj;
            pj["id"] = p.second.id;
            pj["name"] = conv.to_bytes(p.second.name);
            pj["level"] = p.second.level;
            pj["experience"] = p.second.experience;
            pj["hp"] = p.second.hp;
            pj["attack"] = p.second.attack;
            pj["defense"] = p.second.defense;

            for (const auto& m : p.second.moves) {
                pj["moves"].push_back({
                    { "name",  conv.to_bytes(m.name)  },
                    { "power", m.power                },
                    { "accuracy", m.accuracy          },
                    { "type",  conv.to_bytes(m.type)  }
                    });
            }
            arr.push_back(pj);
        }
        root["pokemons"] = arr;

        std::ofstream out(filename);
        if (!out.is_open()) return false;
        out << root.dump(4);
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
            return &(it->second);
        }
        return nullptr;
    }

    Pokemon* GetRedPokemon()
    {
        if (!Redpokemons.empty()) {
            auto it = Redpokemons.begin();
            return &(it->second);
        }
        return nullptr;
    }

    void MyPokeClear() {
        Mypokemons.clear();
    }

    void AddMyPokemon(const Pokemon& p) {
        Mypokemons[p.id] = p;
    }

    std::unordered_map<int, Pokemon> GetMyPokemons() const { return Mypokemons; }

    const std::unordered_map<int, Pokemon>& GetMyPokemonsRef() const {
        return Mypokemons;
    }

    Pokemon& GetMyPokemonRef(int id) { return Mypokemons.at(id); }
    const Pokemon& GetMyPokemonRef(int id) const { return Mypokemons.at(id); }
    //const PokePtr& GetMyPokemonPtr(int id) { return Mpokemons.at(id); }

private:
    std::unordered_map<int, Pokemon> pokemons;
    std::unordered_map<int, Pokemon> Mypokemons;
    std::unordered_map<int, Pokemon> Redpokemons;
    //std::unordered_map<int, PokePtr> Mpokemons;

    PokemonDB() = default;
    ~PokemonDB() = default;
    PokemonDB(const PokemonDB&) = delete;
    PokemonDB& operator=(const PokemonDB&) = delete;
};




