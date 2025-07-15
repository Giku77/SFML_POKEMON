#pragma once

#define DEF_DEV

enum class SortingLayers
{
	Background,
	Foreground,
	Default,
	UI,
};

enum class Origins
{
	TL, TC, TR,	// 0 1 2
	ML, MC, MR,	// 3 4 5
	BL, BC, BR, // 6 7 8
	Custom
};

enum class SceneIds
{
	None = -1,
	Dev1,
	Dev2,
	Dev3,
	Intro,
	Game,
	Battle,
	End,
	Count,
};

enum class Sides
{
	Left,
	Right,
	None,
};

struct Move {
	int id;
	std::wstring name;
	int power;
	int accuracy;
	std::wstring type;
};

struct Pokemon {
	int id;
	std::wstring name;
	int level;
	int experience;
	int hp;
	int attack;
	int defense;
	std::vector<Move*> moves;
	int getMoveSize() const { return moves.size() - 1; }
	bool IsDead() const { return hp <= 0; }
	void TakeDamage(int d) { 
		hp -= d; 
		if (hp < 0) hp = 0;
	}
};

enum class TileType {
	Empty = -1,
	Grass,
	Wall,
	Water,
	BuildingEntrance,
};

struct Tile {
	TileType type;
	sf::Sprite sprite;
	bool isCollidable = false;
	bool isEnterable = false;
};