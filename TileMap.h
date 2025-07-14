#pragma once

struct TileInfo {
    sf::Sprite sprite;
    bool isCollidable = false;
    bool isEnterable = false;
};
struct TileLayer {
    std::vector<TileInfo> tiles;   // width * height
};
struct TilesetInfo {
    int firstgid = 0;
    int columns = 0;
    sf::Texture  texture;
    std::unordered_set<int> collidableLocalIds;
    std::unordered_set<bool> enterLocalIds;
};

class TileMap {
public:

    bool load(const std::string& jsonPath);
    void draw(sf::RenderWindow& window) const;
    bool isCollidable(int x, int y) const;
    bool isEnterable(int x, int y) const;
    sf::Vector2f GetMapPixelSize() const {
        return sf::Vector2f(mapWidth * tileW, mapHeight * tileH);
    }
    int getTileW() const { return tileW; }
    int getTileH() const { return tileH; }

private:
    const TilesetInfo* findTileset(int gid) const;

    int mapWidth = 0, mapHeight = 0;
    int tileW = 0, tileH = 0;
    std::vector<TilesetInfo> tilesets;
    std::vector<TileLayer>   layers;
};

