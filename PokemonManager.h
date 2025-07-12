#pragma once
class PokemonManager {
private:
    std::vector<Pokemon> pokemons;

public:
    void AddPokemon(const Pokemon& p) {
        pokemons.push_back(p);
    }

    const std::vector<Pokemon>& GetAll() const {
        return pokemons;
    }

    void Clear() {
        pokemons.clear();
    }

    //bool SaveToFile(const std::string& filename) const;
    bool SaveGame(const std::wstring& playerName, const std::string& filename) const;
    bool LoadGame(std::wstring& playerName, const std::string& filename);
};

