#pragma once
#include "GameObject.h"
#include "TileMap.h"

class TileMapGameObject : public GameObject
{
protected:
	TileMap tileMap;
public:
	TileMapGameObject(const std::string& name = "");
	virtual ~TileMapGameObject() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::Vector2f GetMapSize() const {
		return tileMap.GetMapPixelSize();
	}
	bool isCollidable(int x, int y) const {
		return tileMap.isCollidable(x, y);
	}
	bool isCenterEnterable(int x, int y) const {
		return tileMap.isCenterEnterable(x, y);
	}
	bool isShopEnterable(int x, int y) const {
		return tileMap.isShopEnterable(x, y);
	}
	bool isBattleable(int x, int y) const {
		return tileMap.isNpcBattleable(x, y);
	}
	bool isPosBattleable(int x, int y) const {
		return tileMap.isPosBattleable(x, y);
	}
	int getTileW() const { return tileMap.getTileW(); }
	int getTileH() const { return tileMap.getTileH(); }
};

