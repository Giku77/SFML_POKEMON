#pragma once
#include "TileMap.h"
class Player
{
private:
	sf::Vector2i tilePos;

public:
	void Move(int dx, int dy, const TileMap& map);
	const sf::Vector2i& GetTilePos() const;
};

