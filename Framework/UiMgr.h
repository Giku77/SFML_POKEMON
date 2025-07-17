#pragma once

class UI;

class UiMgr
{
	//friend class Singleton<UiMgr>;

protected:

	std::vector<UI*> uiele;

public:
	UiMgr() = default;
	~UiMgr() = default;

	void Add(UI* ui);
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);

	void Clear()
	{
		for (UI* ui : uiele)
			delete ui;
		uiele.clear();
	}
};

//#define UI_MGR (UiMgr::Instance())



