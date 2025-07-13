#include "stdafx.h"
#include "Player.h"

void Player::Move(int dx, int dy, const TileMap& map)
{
	sf::Vector2i newPos = tilePos + sf::Vector2i(dx, dy);
	if (!map.isCollidable(newPos.x, newPos.y)) {
		tilePos = newPos;
	}
}
