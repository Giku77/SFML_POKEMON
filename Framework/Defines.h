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
	int pp;
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
	std::vector<Move> moves;
	int getMoveSize() const { return moves.size(); }
	bool IsDead() const { return hp <= 0; }
	void TakeDamage(int d) { 
		hp -= d; 
		if (hp < 0) hp = 0;
	}
	void ApplyLevelScaling(int baseLevel = 5)
	{
		if (level <= baseLevel) {
			return;
		}

		const int  dLvl = level - baseLevel;
		constexpr float HP_RATE = 0.05f;  // 레벨당 +5 %
		constexpr float ATK_RATE = 0.03f;  // 레벨당 +3 %
		constexpr float DEF_RATE = 0.025f; // 레벨당 +2.5 %

		hp *= static_cast<int>(1.f + dLvl * HP_RATE);
		attack *= static_cast<int>(1.f + dLvl * ATK_RATE);
		defense *= static_cast<int>(1.f + dLvl * DEF_RATE);
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