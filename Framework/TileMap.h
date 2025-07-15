#pragma once

struct TileInfo {
    sf::Sprite sprite;
    bool isCollidable = false;
    bool isEnterable = false;
    bool isCenterEnter = false;
    bool isShopEnter = false;
    bool isBattle = false;
};
struct TileLayer {
    std::vector<TileInfo> tiles;   // width * height
};
struct TilesetInfo {
    int firstgid = 0;
    int columns = 0;
    sf::Texture  texture;
    std::unordered_set<int> collidableLocalIds;
    std::unordered_set<int> enterCenterLocalIds;
    std::unordered_set<int> enterShopLocalIds;
    std::unordered_set<int> battleNpcLocalIds;
};

class TileMap {
public:

    bool load(const std::string& jsonPath);
    void draw(sf::RenderWindow& window) const;
    bool isCollidable(int x, int y) const;
    bool isCenterEnterable(int x, int y) const;
    bool isShopEnterable(int x, int y) const;
    bool isNpcBattleable(int x, int y) const;
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

