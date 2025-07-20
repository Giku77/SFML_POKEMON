#pragma once
#include "UI.h"
#include "Text.h"
#include "Button.h"
#include "ItemDB.h"

class ItemSlot;
class UiMgr;
class Inventory;

class ShopUI : public UI
{
public:
    ShopUI(const std::string& name = "Shop");
    ~ShopUI() override;

    void Init()   override;
    void Release() override;
    void Reset()   override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void Open(const std::string& shopTag, int& playerGold, Inventory& inventory);
    void Close();
    bool IsOpen() const { return active; }

private:
    Inventory* inv = nullptr;
    sf::Sprite panelBg;               
    sf::Texture tex;
    Text title;
    Text goldText;
    Text infoText;                    

    Button closeBtn;
    Button buyBtn;
    Button sellBtn;

    UiMgr* Uimgr = nullptr;

    std::vector<ItemSlot*> slots;          
    int  selectedIdx = -1;              
    int  firstRow = 0;                 
    int  visibleRows = 6;

    std::vector<const ItemData*> items;             
    int* pPlayerGold = nullptr;          

    void RefreshGold();
    void RefreshSlots();
    void OnSlotClicked(int rowInView);
    void OnBuyClicked();
    void OnSellClicked();                   // (선택) 나중에
};