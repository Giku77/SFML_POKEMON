#pragma once
class Building
{
protected:
	sf::Vector2i position;
	sf::Vector2i size;
	std::wstring name;


public:
	Building(const sf::Vector2i& pos, const sf::Vector2i& size, const std::wstring& name);

	virtual void Interact();
	bool IsInside(const sf::Vector2i& tilePos) const;

};

class HealCenter : public Building {
public:
	using Building::Building;
	void Interact() override {

	}
};

class Shop : public Building {
public:
	using Building::Building;
	void Interact() override {

	}
};
