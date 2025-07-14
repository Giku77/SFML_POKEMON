#pragma once
#include "UI.h"
#include "Text.h"
#include "Button.h"
#include "ItemDB.h"

class ItemSlot;

/* 1-줄 스크롤 리스트 + 상세·버튼 패널 */
class ShopUI : public UI
{
public:
    ShopUI(const std::string& name = "Shop");
    ~ShopUI() override;

    // UI
    void Init()   override;
    void Release() override;
    void Reset()   override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // API
    void Open(const std::string& shopTag, int playerGold);
    void Close();
    bool IsOpen() const { return active; }

private:
    // 내부 데이터
    sf::Sprite panelBg;                     // 전체 창
    Text       title;
    Text       goldText;
    Text       infoText;                    // 아이템 설명

    Button     closeBtn;
    Button     buyBtn;
    Button     sellBtn;

    std::vector<ItemSlot*> slots;           // 화면에 보이는 슬롯 오브젝트
    int  selectedIdx = -1;                // items[] 기준
    int  firstRow = 0;                 // 스크롤용
    int  visibleRows = 6;

    std::vector<const ItemData*> items;             // 현재 상점 취급품
    int* pPlayerGold = nullptr;           // 외부 소지금 포인터

    // 내부 헬퍼
    void RefreshGold();
    void RefreshSlots();
    void OnSlotClicked(int rowInView);
    void OnBuyClicked();
    void OnSellClicked();                   // (선택) 나중에
};