#pragma once
#include "UI.h"
#include "Text.h"
#include "Button.h"
#include "ItemDB.h"

class ItemSlot;

/* 1-�� ��ũ�� ����Ʈ + �󼼡���ư �г� */
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
    // ���� ������
    sf::Sprite panelBg;                     // ��ü â
    Text       title;
    Text       goldText;
    Text       infoText;                    // ������ ����

    Button     closeBtn;
    Button     buyBtn;
    Button     sellBtn;

    std::vector<ItemSlot*> slots;           // ȭ�鿡 ���̴� ���� ������Ʈ
    int  selectedIdx = -1;                // items[] ����
    int  firstRow = 0;                 // ��ũ�ѿ�
    int  visibleRows = 6;

    std::vector<const ItemData*> items;             // ���� ���� ���ǰ
    int* pPlayerGold = nullptr;           // �ܺ� ������ ������

    // ���� ����
    void RefreshGold();
    void RefreshSlots();
    void OnSlotClicked(int rowInView);
    void OnBuyClicked();
    void OnSellClicked();                   // (����) ���߿�
};