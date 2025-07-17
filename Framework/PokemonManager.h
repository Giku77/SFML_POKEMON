#pragma once
class PokemonManager {
private:
    std::unordered_map<int, Pokemon> pokemons;

public:
    void AddPokemon(const Pokemon& p) {
        pokemons[p.id] = p;
    }

    void SetPokemonAndSave(const Pokemon& p, std::wstring& playerName, const std::string& filename) {
        LoadGame(playerName, filename);
        pokemons[p.id] = p;
        SaveGame(playerName, filename);
    }

    const std::unordered_map<int, Pokemon>& GetAll() const {
        return pokemons;
    }

    void Clear() {
        pokemons.clear();
    }

    //bool SaveToFile(const std::string& filename) const;
    bool SaveGame(const std::wstring& playerName, const std::string& filename) const;
    bool LoadGame(std::wstring& playerName, const std::string& filename);
};

