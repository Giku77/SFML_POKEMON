#include "stdafx.h"
#include "SceneIntro.h"
#include "Text.h"
#include "BattleTransition.cpp"
#include "SpriteAnimator.h"


SceneIntro::SceneIntro()
	: Scene(SceneIds::Intro)
{
}

SceneIntro::~SceneIntro()
{
	delete StartText;
	StartText = nullptr;
	delete intro;
	intro = nullptr;
}

void SceneIntro::Init()
{
	//std::cout << "init 입장" << std::endl;
	soundIds.push_back("sounds/Red_T.wav");
	soundIds.push_back("sounds/TitleClick.wav");
	//newTex.loadFromFile("graphics/Title.png");
	//newScreen.setTexture(newTex);
	//sf::Vector2f pos = { 0.f,  0.f };
	//newScreen.setPosition(ScreenToUi((sf::Vector2i)pos));
	//newScreen.setScale(1.3f, 0.7f);
	newTex.loadFromFile("graphics/Tit.png");
	newScreen.setTexture(newTex);
	sf::Vector2f pos = { 0.f,  0.f };
	newScreen.setPosition(ScreenToUi((sf::Vector2i)pos));
	newScreen.setScale(2.6f, 1.65f);
	intro = new SpriteAnimator(&newScreen, newTex, 500.f, 448.f, 0.06f);

	titleTex.loadFromFile("graphics/pn.png");
	titleScreen.setTexture(titleTex);
	sf::Vector2f tpos = { 470.f,  10.f };
	titleScreen.setPosition(ScreenToUi((sf::Vector2i)tpos));
	titleScreen.setScale(0.28f, 0.2f);

	StartText = new Text();
	StartText->SetFillColor(sf::Color::Black);
	StartText->AddText("fonts/pokemon-dppt.otf", L"눌러서 시작!", 80, ScreenToUi(sf::Vector2i(850, 460)));
	StartText->Reset();
	Scene::Init();
}

void SceneIntro::Enter()
{
	Scene::Enter();
	//std::cout << "메인 입장" << std::endl;
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("sounds/Red_T.wav"), true);
}

void SceneIntro::Update(float dt)
{
	Scene::Update(dt);
	////if (transs)
	////{
	////	if (transs->update(dt)) {
	////		//std::cout << "애니 업데이트" << std::endl;
	////		//delete transs;
	////		//transs = nullptr;
	////		//SCENE_MGR.ChangeScene(SceneIds::Dev3);
	////		return;
	////	}
	////}
	introTime += dt;
	if (introTime > 1.f && isBgDraw) {
		isTextDraw ? isTextDraw = false : isTextDraw = true;
		introTime = 0.f;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space)) {
		//if (!transs) transs = new BattleTransition(FRAMEWORK.GetWindowSize(), 1, uiView);
		SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("sounds/TitleClick.wav"), false);
		isBgDraw = false;
		isTextDraw = false;
	}
	if (!isBgDraw) {
		bgTime += dt;
		if (bgTime > 1.f) {
			SCENE_MGR.ChangeScene(SceneIds::Dev3);
		}
	}
	if (intro->getIndex() == 58) {
		intro->setIndex(0);
	}
	intro->Update(dt, true);
	
}

void SceneIntro::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(window.getDefaultView());
	if(isBgDraw) window.draw(newScreen);
	if (isBgDraw) window.draw(titleScreen);
	if(isTextDraw) StartText->Draw(window);
	//window.setView(uiView);
	//if(transs) transs->draw(window);
}
