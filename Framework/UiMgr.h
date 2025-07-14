#pragma once

class UI;

class UiMgr : public Singleton<UiMgr>
{
	friend class Singleton<UiMgr>;

protected:
	UiMgr() = default;
	~UiMgr() = default;

	std::vector<UI*> uiele;

public:


	void Add(UI* ui);
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
};

#define UI_MGR (UiMgr::Instance())



