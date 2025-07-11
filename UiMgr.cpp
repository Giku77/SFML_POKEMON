#include "stdafx.h"
#include "UiMgr.h"
#include "Text.h"
#include "Button.h"



void UiMgr::Add(UI* ui)
{
	uiele.push_back(ui);
}

void UiMgr::Update(float dt)
{
	for (auto u : uiele) {
		if (u->GetActive()) {
			u->Update(dt);
		}
	}
}

void UiMgr::Init() {
	for (auto u : uiele) {
		if (u->GetActive()) {
			u->Reset();
		}
	}
}

void UiMgr::Draw(sf::RenderWindow& window)
{
	for (auto u : uiele) {
		if (u->GetActive()) {
			u->Draw(window);
		}
	}
}
