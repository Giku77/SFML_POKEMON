#include "stdafx.h"
#include "PokemonManager.h"

bool PokemonManager::SaveGame(const std::wstring& playerName,
    const std::string& filename) const
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

    nlohmann::json root;
    root["player"] = conv.to_bytes(playerName);  

    nlohmann::json arr;                           
    for (const auto& p : pokemons) {
        nlohmann::json pj;
        pj["id"] = p.id;
        pj["name"] = conv.to_bytes(p.name);
        pj["level"] = p.level;
        pj["experience"] = p.experience;
        pj["hp"] = p.hp;
        pj["attack"] = p.attack;
        pj["defense"] = p.defense;

        for (const auto& m : p.moves) {
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

bool PokemonManager::LoadGame(std::wstring& playerName,
    const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    nlohmann::json root;
    in >> root;

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    playerName = conv.from_bytes(root.at("player").get<std::string>());

    pokemons.clear();
    for (const auto& pj : root.at("pokemons")) {
        Pokemon p;
        p.id = pj.at("id").get<int>();
        p.name = conv.from_bytes(pj.at("name").get<std::string>());
        p.level = pj.at("level").get<int>();
        p.experience = pj.at("experience").get<int>();
        p.hp = pj.at("hp").get<int>();
        p.attack = pj.at("attack").get<int>();
        p.defense = pj.at("defense").get<int>();

        for (const auto& mj : pj.at("moves")) {
            Move m;
            m.name = conv.from_bytes(mj.at("name").get<std::string>());
            m.power = mj.at("power").get<int>();
            m.accuracy = mj.at("accuracy").get<int>();
            m.type = conv.from_bytes(mj.at("type").get<std::string>());
            p.moves.push_back(m);
        }
        pokemons.push_back(p);
    }
    return true;
}
