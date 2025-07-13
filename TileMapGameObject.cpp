#include "stdafx.h"
#include "TileMapGameObject.h"

TileMapGameObject::TileMapGameObject(const std::string& name)
	: GameObject(name)
{
}

void TileMapGameObject::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
}

void TileMapGameObject::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
}

void TileMapGameObject::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
}

void TileMapGameObject::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
}

void TileMapGameObject::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	/*if (preset != Origins::Custom)
	{
		Utils::SetOrigin(gameObject, preset);
	}*/
}

void TileMapGameObject::Init()
{
	tileMap.load("data/world.tmj");
}

void TileMapGameObject::Release()
{
}

void TileMapGameObject::Reset()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 0;
}

void TileMapGameObject::Update(float dt)
{
}

void TileMapGameObject::Draw(sf::RenderWindow& window)
{
	tileMap.draw(window);
}