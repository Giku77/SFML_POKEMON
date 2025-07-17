#include "stdafx.h"
#include "ScenePokeDev.h"
#include "Button.h"
#include "Text.h"
#include "MessageMgr.h"
#include "StoryPlayer.h"
#include "SpriteAnimator.h"
#include "UiMgr.h"
#include "PokemonDB.h"

MessageMgr msgMgr;
StoryPlayer story;

ScenePokeDev::ScenePokeDev()
	: Scene(SceneIds::Dev3)
{
}

ScenePokeDev::~ScenePokeDev()
{
	delete button;
	button = nullptr;
	delete text;
	text = nullptr;
	delete anim;
	anim = nullptr;
}

void ScenePokeDev::SetAnimateScreen(sf::Sprite& s, int a)
{
	sf::Color c = s.getColor();
	c.a = a;
	s.setColor(c);
}

void ScenePokeDev::SetBackground(const std::string& path, const sf::IntRect& rect, sf::Sprite& s, sf::Texture& t, const Type& type, const sf::Vector2f& v)
{
	if (!t.loadFromFile(path)) {
		std::cerr << "Failed to load background texture: " << path << std::endl;
		return;
	}

	s.setTexture(t);
	s.setTextureRect(rect);

	sf::Vector2f bgPos = { 0.f, 0.f };
	s.setPosition(ScreenToUi(static_cast<sf::Vector2i>(bgPos)));

	sf::Vector2u windowSize = FRAMEWORK.GetWindow().getSize();
	sf::Vector2u textureSize(rect.width, rect.height);

	if (type == Type::BG) {
		float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
		float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
		s.setScale(scaleX, scaleY);
	}
	else s.setScale(v.x, v.y);
}


void ScenePokeDev::Init()
{
	msgMgr.Load("data/messages.json");
	story.Load("intro", msgMgr);
	texIds.push_back("graphics/18507.png");
	texIds.push_back("graphics/inputbox.png");
	fontIds.push_back("fonts/pokemon-dppt.otf");

	Uimgr = new UiMgr();

	button = new Button("TestButton", true);
	button->SetBackGroud("graphics/18507.png", 4.f, 4.f);
	sf::String s;
	sf::String ss;
	button->SetOnClick([=]() mutable {
		if (!story.IsFinished()) {
			switch (story.getIndex())
			{
			case 1:
				s = story.Next();
				isNameInputActive = true;
				InputMgr::isInputBuffer = true;
				break;
			case 2:
				s = InputMgr::GetinputBuffer() + story.Next();
				text->SetActive(false);
				isNameInputActive = false;
				InputMgr::isInputBuffer = false;
				break;
			case 3:
				s = story.Next();
				ss = s;
				isChoosePokemon = true;
				break;
			case 4:
				if (!isChoosePokemon) s = story.Next();
				else s = ss;
				break;
			default:
				s = story.Next();
				break;
			}
			button->SetString(s);
			std::cout << "ÀÎµ¦½º : " << story.getIndex() << std::endl;
		}
		});
	button->SetButton({ FRAMEWORK.GetWindowSize().x / 2.f + 200.f, 300.f }, sf::Color::Black, "fonts/pokemon-dppt.otf");
	//std::wstring welcome = msgMgr.Get("intro.welcome");
	//button->AddButton(msgMgr.Get("intro.welcome"), 40, sf::Color::Black);
	button->AddButton(story.GetCurrent(), 40, sf::Color::Black);
	story.Next();
	sf::Vector2f pos = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y - 100.f };
	//sf::Vector2f pos = { 0.f, FRAMEWORK.GetWindowSize().y - 60.f };
	button->SetPosition(ScreenToUi((sf::Vector2i)pos));
	button->TextSetPosition(ScreenToUi((sf::Vector2i)sf::Vector2f(50.f, pos.y - 50.f)));
	Uimgr->Add(button);

	text = new Text("Test");
	sf::Vector2f pos2 = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f};

	text->SetBackGround("graphics/inputbox.png", {135, 60, 237, 45}, 3.f, 4.f);
	text->SetFillColor(sf::Color::Black);
	text->AddText("fonts/pokemon-dppt.otf", "", 50, ScreenToUi((sf::Vector2i)pos2));
	text->GetText().setPosition({ text->GetPosition().x, text->GetPosition().y - 20.f });
	text->SetActive(false);
	Uimgr->Add(text);

	SetBackground("graphics/6960.png", { 10, 510, 240, 190 }, backGround, bGTex, Type::BG);
	SetBackground("graphics/6960-.png", { 45, 80, 75, 130 }, backGround2, bGTex2, Type::CS, {3.f, 3.f});
	
	ballTex.loadFromFile("graphics/ball-.png");
	balls.setTexture(ballTex);
	balls.setPosition(ScreenToUi((sf::Vector2i) (pos2 - sf::Vector2f(500.f, 200.f))));
	balls.setScale(1.f, 1.f);

	std::vector<sf::IntRect> rects = {
		{0, 0, 163, 163},
		{163, 0, 163, 163},
		{326, 0, 163, 163},
	};

	anim = new SpriteAnimator(&balls, rects, 0.15f);

	backGround2.setPosition(ScreenToUi((sf::Vector2i) sf::Vector2f(pos2.x, 50.f)));

	Scene::Init();
	Uimgr->Init();
}

void ScenePokeDev::Enter()
{
	Scene::Enter();
}

void ScenePokeDev::Update(float dt)
{
	Scene::Update(dt);
	Uimgr->Update(dt);
	if (isNameInputActive) {
		text->Reset();
		text->SetActive(true);
		if (!InputMgr::GetinputBuffer().empty()) {
			text->SetString(InputMgr::GetinputBuffer());
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Enter)) {
			button->SetString(InputMgr::GetinputBuffer() + story.Next());
			text->SetActive(false);
			isNameInputActive = false;
			InputMgr::isInputBuffer = false;
		}
	}


	sf::Vector2f mouseWorldPos = FRAMEWORK.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(FRAMEWORK.GetWindow()));
	bool isMouseOver = Utils::PointInTransformBounds(balls, balls.getLocalBounds(), mouseWorldPos);

	if (isChoosePokemon && isMouseOver && InputMgr::GetMouseButtonDown(sf::Mouse::Left)) {
		isChoosePokemon = false;
		float r = Utils::RandomValue();
		if (r < 0.33f) RandPokemon = 1;
		else if (r < 0.66f) RandPokemon = 4;
		else if (r < 1.f) RandPokemon = 7;

		PokemonManager pm;
		PokemonDB::Instance().LoadFromJson("data/_pokemon_001-151.json");

		//Move scratch = { 10, L"ÇÒÄû±â", 40, 100, 0, L"³ë¸»" };
		//newPoke.moves.push_back(scratch);

		pm.AddPokemon(*PokemonDB::Instance().GetPokemon(RandPokemon));
		pm.SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
		button->SetString(PokemonDB::Instance().GetPokemon(RandPokemon)->name + story.Next());
	}

	anim->Update(dt, true);
	if (story.IsFinished()) {
		SceneAnimateTime += dt;
		//std::cout << SceneAnimateTime << std::endl;
		SetAnimateScreen(backGround, 255 - (SceneAnimateTime * 100.f));
		SetAnimateScreen(backGround2, 255 - (SceneAnimateTime * 100.f));

		if(SceneAnimateTime > 2.f )SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}

void ScenePokeDev::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	window.draw(backGround);
	window.draw(backGround2);
	if(isChoosePokemon) window.draw(balls);
	Scene::Draw(window);
	Uimgr->Draw(window);
}