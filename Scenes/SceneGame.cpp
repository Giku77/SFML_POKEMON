#include "stdafx.h"
#include "SceneGame.h"


SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	Scene::Init();
}

void SceneGame::Enter()
{
	Scene::Enter();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}