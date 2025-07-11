#include "stdafx.h"
#include "SceneBattle.h"

SceneBattle::SceneBattle()
	: Scene(SceneIds::Battle)
{
}

void SceneBattle::Init()
{
	Scene::Init();
}

void SceneBattle::Enter()
{
	Scene::Enter();
}

void SceneBattle::Update(float dt)
{
	Scene::Update(dt);
}