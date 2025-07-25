﻿#include "stdafx.h"
#include "TileMap.h"
#include "tinyxml2.h"

static std::string getDir(const std::string& path)
{
    const char* p = strrchr(path.c_str(), '/');
#ifdef _WIN32
    const char* q = strrchr(path.c_str(), '\\');
    if (!p || (q && q > p)) p = q;
#endif
    return p ? std::string(path.c_str(), p - path.c_str()) : "";
}

static bool loadTilesetEmbedded(const std::string& baseDir,   
    const nlohmann::json& tsJson,
    TilesetInfo& outTs)
{
    if (!tsJson.contains("image")) {
        std::cerr << "[Tileset] image 필드가 없습니다 (Embed Tilesets 필요)\n";
        return false;
    }
    if (tsJson.contains("tiles")) {
        for (const auto& tile : tsJson["tiles"]) {
            int localId = tile["id"];
            if (tile.contains("objectgroup")) {
                outTs.collidableLocalIds.insert(localId);
            }
            if (tile.contains("properties")) {
                for (const auto& prop : tile["properties"]) {
                    if (prop.contains("name") && prop["name"] == "isCenterEnter" &&
                        prop.contains("type") && prop["type"] == "bool" &&
                        prop.contains("value") && prop["value"] == true)
                    {
                        outTs.enterCenterLocalIds.insert(localId);
                    }
                    if (prop.contains("name") && prop["name"] == "isShopEnter" &&
                        prop.contains("type") && prop["type"] == "bool" &&
                        prop.contains("value") && prop["value"] == true)
                    {
                        outTs.enterShopLocalIds.insert(localId);
                    }
                    if (prop.contains("name") && prop["name"] == "isBattle" &&
                        prop.contains("type") && prop["type"] == "bool" &&
                        prop.contains("value") && prop["value"] == true)
                    {
                        outTs.battleNpcLocalIds.insert(localId);
                    }
                    if (prop.contains("name") && prop["name"] == "isRandBattle" &&
                        prop.contains("type") && prop["type"] == "bool" &&
                        prop.contains("value") && prop["value"] == true)
                    {
                        outTs.battlePosLocalIds.insert(localId);
                    }
                    if (prop.contains("name") && prop["name"] == "NPC" &&
                        prop.contains("type") && prop["type"] == "string" &&
                        prop.contains("value"))
                    {
                        std::string npcId = prop["value"];
                        outTs.npcLocalIdMap[localId] = npcId;   
                    }

                }
            }
        }
    }
    std::string imgRel = tsJson["image"];                 
    std::string imgPath = baseDir.empty() ? imgRel        
        : (baseDir + "/" + imgRel);   

    if (!outTs.texture.loadFromFile(imgPath)) {
        std::cerr << "[Tileset] texture 로드 실패: " << imgPath << '\n';
        return false;
    }
    outTs.columns = tsJson["columns"];
    outTs.firstgid = tsJson["firstgid"];
    std::cout << "[Tileset] OK  " << imgPath
        << "  firstgid=" << outTs.firstgid
        << "  columns=" << outTs.columns << '\n';
    return true;
}

bool TileMap::load(const std::string& jsonPath)
{
    std::cout << "[DBG] load " << jsonPath << '\n';
    std::ifstream in(jsonPath);
    if (!in.is_open()) { std::cerr << "맵 열기 실패: " << jsonPath << '\n'; return false; }

    nlohmann::json j;  in >> j;

    /*std::cout << "──── json dump ────\n"
        << j.dump(2) << '\n'
        << "──────────────────\n";*/

    mapWidth = j["width"];
    mapHeight = j["height"];
    tileW = j["tilewidth"];
    tileH = j["tileheight"];
    std::cout << "[DBG] map " << mapWidth << 'x' << mapHeight
        << "  tile " << tileW << 'x' << tileH << '\n';


    std::string baseDir = getDir(jsonPath);               

    tilesets.clear();
    for (auto& tsJ : j["tilesets"]) {
        TilesetInfo ts;
        std::cout << "[DBG] tileset firstgid=" << tsJ["firstgid"] << '\n';
        if (!loadTilesetEmbedded(baseDir, tsJ, ts)) return false;   
        tilesets.push_back(std::move(ts));
    }

    layers.clear();
    for (auto& lJ : j["layers"]) {
        std::cout << "[DBG] layer " << lJ["name"] << " data=" << lJ["data"].size() << '\n';
        if (lJ["type"] != "tilelayer" || !lJ["visible"]) continue;

        TileLayer layer;
        layer.tiles.resize(mapWidth * mapHeight);
        const auto& data = lJ["data"];

        for (int idx = 0; idx < data.size(); ++idx) {
            int gid = data[idx];
            if (gid == 0) continue;

            const TilesetInfo* ts = findTileset(gid);
            if (!ts) continue;

            int local = gid - ts->firstgid;
            int col = local % ts->columns;
            int row = local / ts->columns;

            TileInfo& t = layer.tiles[idx];
            t.sprite.setTexture(ts->texture);
            t.sprite.setTextureRect({ col * tileW, row * tileH, tileW, tileH });
            t.isCollidable = ts->collidableLocalIds.count(local) > 0;
            //t.isEnterable = ts->enterLocalIds.count(local) > 0;
            t.isCenterEnter = ts->enterCenterLocalIds.count(local) > 0;
            t.isShopEnter = ts->enterShopLocalIds.count(local) > 0;
            t.isBattle = ts->battleNpcLocalIds.count(local) > 0;
            t.isPosBattle = ts->battlePosLocalIds.count(local) > 0;

            auto itNpc = ts->npcLocalIdMap.find(local);
            if (itNpc != ts->npcLocalIdMap.end())
                t.npcId = itNpc->second;

           /* for (int y = 0; y < mapHeight; ++y)
                for (int x = 0; x < mapWidth; ++x)
                    if (isCenterEnterable(x, y))
                        std::cout << "isCenterEnter at (" << x << ", " << y << ")\n";*/

            int x = idx % mapWidth;
            int y = idx / mapWidth;
            t.sprite.setPosition(float(x * tileW), float(y * tileH));
        }
        layers.emplace_back(std::move(layer));
    }

    std::cout << "[TileMap] 로드 완료: " << layers.size()
        << " layer, " << tilesets.size() << " tileset\n";
    return true;
}

const TilesetInfo* TileMap::findTileset(int gid) const
{
    const TilesetInfo* pick = nullptr;
    for (const auto& ts : tilesets)
        if (gid >= ts.firstgid) pick = &ts; else break;
    return pick;
}

void TileMap::draw(sf::RenderWindow& window) const
{
    for (const auto& layer : layers)
        for (const auto& t : layer.tiles)
            window.draw(t.sprite);
}

bool TileMap::isCollidable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return true;
    int idx = y * mapWidth + x;
    for (const auto& layer : layers)
        if (idx < layer.tiles.size() && layer.tiles[idx].isCollidable)
            return true;
    return false;
}

bool TileMap::isCenterEnterable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return false;
    int idx = y * mapWidth + x;
    for (const auto& layer : layers)
        if (idx < layer.tiles.size() && layer.tiles[idx].isCenterEnter) {
            return true;
        }
    return false;
}

bool TileMap::isShopEnterable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return false;
    int idx = y * mapWidth + x;
    for (const auto& layer : layers)
        if (idx < layer.tiles.size() && layer.tiles[idx].isShopEnter) {
            return true;
        }
    return false;
}

bool TileMap::isNpcBattleable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return false;
    int idx = y * mapWidth + x;
    for (const auto& layer : layers)
        if (idx < layer.tiles.size() && layer.tiles[idx].isBattle) {
            return true;
        }
    return false;
}

bool TileMap::isPosBattleable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return false;
    int idx = y * mapWidth + x;
    for (const auto& layer : layers)
        if (idx < layer.tiles.size() && layer.tiles[idx].isPosBattle) {
            return true;
        }
    return false;
}

std::string TileMap::getNpcId(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) return "";
    int idx = y * mapWidth + x;

    for (const auto& layer : layers)
        if (idx < layer.tiles.size()) {
            const auto& t = layer.tiles[idx];
            if (!t.npcId.empty())         
                return t.npcId;
        }
    return "";
}

