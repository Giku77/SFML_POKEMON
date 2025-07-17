#pragma once
#include "UI.h"
#include "Inventory.h"
#include "ItemDB.h"
class InventoryUI : public UI
{
private:
    static constexpr int cols = 4;
    static constexpr int rows = 4;          
    static constexpr float cellW = 180.f;
    static constexpr float cellH = 50.f;

    sf::Sprite panelBackground;
    sf::Texture panelTex;

    UiMgr mgr;

    Inventory* inventory = nullptr;

    std::vector<ItemSlot*> slots;   // 화면용 슬롯 16개

    void RefreshSlots();
    void OnSlotClicked(int visualIdx);

public:
    InventoryUI(const std::string& name = "InventoryUI");

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void Open(Inventory& inv);     
    void Close() { active = false; }
};

