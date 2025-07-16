#include "stdafx.h"
#include "ShopUI.h"
#include "UiMgr.h"
#include "Inventory.h"

constexpr float PANEL_WIDTH = 450.f;
constexpr float PANEL_HEIGHT = 320.f;
sf::Vector2f PANEL_ORIGIN{ 400.f, 200.f };  


ShopUI::ShopUI(const std::string& name)
    : UI(name)
{
    active = false;         
}

ShopUI::~ShopUI()
{
    Release();
}


void ShopUI::Init()
{
    Uimgr = new UiMgr();
    //tex.loadFromFile("graphics/ScreenBackgrounds.png");
    tex = Utils::loadWithColorKey("graphics/ScreenBackgrounds.png", sf::Color(75, 75, 75));
    panelBg.setTexture(tex);
    panelBg.setTextureRect({307, 410, 564 - 307, 604 - 410 });
    panelBg.setPosition(PANEL_ORIGIN - sf::Vector2f(50.f, 60.f));
    panelBg.setScale(2.f, 2.f);



    sf::Vector2f pos2 = { FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f };

    //text->SetBackGround("graphics/inputbox.png", { 135, 60, 237, 45 }, 3.f, 4.f);

    title.SetFillColor(sf::Color::Black);
    title.AddText("fonts/pokemon-dppt.otf", L"포켓몬 상점", 40, (PANEL_ORIGIN + sf::Vector2f{ 40.f, -30.f }));
    //title.GetText().setPosition({ text->GetPosition().x, text->GetPosition().y - 20.f });
    Uimgr->Add(&title);

    goldText.SetFillColor(sf::Color::Black);
    goldText.AddText("fonts/pokemon-dppt.otf", "", 30, (PANEL_ORIGIN + sf::Vector2f{ 320.f, -20.f }));
    Uimgr->Add(&goldText);

    infoText.SetFillColor(sf::Color::Black);
    infoText.AddText("fonts/pokemon-dppt.otf", " ", 25, (PANEL_ORIGIN + sf::Vector2f{ 70.f, 205.f }));
    Uimgr->Add(&infoText);

 /*   closeBtn.SetButton({ 60.f,28.f }, sf::Color(80, 80, 80, 200), "fonts/pokemon-dppt.otf");
    closeBtn.SetString("X");
    closeBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ PANEL_WIDTH - 50.f, -25.f });
    closeBtn.TextSetPosition(closeBtn.GetPosition() - sf::Vector2f(0.f, 10.f));
    closeBtn.SetOnClick([this]() { Close(); });
    Uimgr->Add(&closeBtn);*/



    buyBtn.SetButton({ 90.f,32.f }, sf::Color(0, 120, 255, 200), "fonts/pokemon-dppt.otf");
    buyBtn.isMouseOverColor = true;
    buyBtn.SetOutlineColor(sf::Color::Black);
    buyBtn.AddButton(L"구매", 30, sf::Color::Black);
    buyBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 385.f, 220.f });
    buyBtn.TextSetPosition(buyBtn.GetPosition() - sf::Vector2f(20.f, 15.f));
    buyBtn.SetOnClick([this]() { OnBuyClicked(); });
    Uimgr->Add(&buyBtn);

    sellBtn.SetButton({ 90.f,32.f }, sf::Color(0, 200, 100, 200), "fonts/pokemon-dppt.otf");
    sellBtn.isMouseOverColor = true;
    sellBtn.SetOutlineColor(sf::Color::Black);
    sellBtn.AddButton(L"판매", 30, sf::Color::Black);
    sellBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 385.f, 260.f });
    sellBtn.TextSetPosition(sellBtn.GetPosition() - sf::Vector2f(20.f, 15.f));
    sellBtn.SetOnClick([this]() { OnSellClicked(); });
    Uimgr->Add(&sellBtn);
    Uimgr->Init();

    const float rowHeight = 50.f;
    for (int r = 0; r < visibleRows; ++r)
    {
        auto* slot = new ItemSlot("Slot_" + std::to_string(r));
        slot->Init();
        slot->SetPosition(PANEL_ORIGIN + sf::Vector2f{ 150.f, 50.f + r * rowHeight });
        slot->isMouseOverColor = true;
        slot->SetOnSelect([this, r]() { OnSlotClicked(r); });
        slots.push_back(slot);
    }
    for (auto* s : slots) s->Init();
}

void ShopUI::Release()
{
    for (auto* s : slots) { delete s; }
    slots.clear();
}

void ShopUI::Reset()
{
    selectedIdx = -1;
    firstRow = 0;
    //infoText.SetString(" ");
    RefreshSlots();
}


void ShopUI::Open(const std::string& shopTag, int& playerGold, Inventory& inventory)
{
    ItemDB::Instance().LoadFromJson("data/shop.json");
    items = ItemDB::Instance().GetShopItems(shopTag);
    std::cout << "아이템 사이즈 : " << items.size() << std::endl;
    pPlayerGold = &playerGold;  
    inv = &inventory;
    RefreshGold();
    Reset();
    active = true;
}

void ShopUI::Close()
{
    active = false;
}


void ShopUI::Update(float dt)
{
    if (!active) return;
    Uimgr->Update(dt);

    float wheel = InputMgr::GetMouseWheelDelta();
    //std::cout << "휠 : " << wheel << std::endl;
    if (wheel != 0.f && items.size() > visibleRows)
    {
        firstRow = Utils::Clamp(firstRow - wheel, 0, int(items.size() - visibleRows));
        RefreshSlots();
    }

    for (auto* s : slots) s->Update(dt);
}

void ShopUI::Draw(sf::RenderWindow& window)
{
    if (!active) return;

    window.draw(panelBg);
    Uimgr->Draw(window);

    for (auto* s : slots) s->Draw(window);


}

void ShopUI::RefreshGold()
{
    goldText.SetString(L"골드 : " + std::to_wstring(*pPlayerGold));
}

void ShopUI::RefreshSlots()
{
    for (int i = 0; i < visibleRows; ++i)
    {
        int itemIdx = firstRow + i;
        if (itemIdx < (int)items.size())
        {
            slots[i]->SetActive(true);
            slots[i]->SetItem(items[itemIdx]);
            slots[i]->SetSelected(itemIdx == selectedIdx);
        }
        else
        {
            slots[i]->SetActive(false);
        }
    }
}


void ShopUI::OnSlotClicked(int rowInView)
{
    int idx = firstRow + rowInView;
    std::cout << "슬롯 : " << idx << std::endl;
    if (idx >= (int)items.size()) return;

    selectedIdx = idx;
    RefreshSlots();

    std::cout << "클릭 슬롯" << std::endl;
    const auto& data = items[idx];
    infoText.SetString(data->desc);
}

void ShopUI::OnBuyClicked()
{
    if (selectedIdx < 0) return;

    auto& item = items[selectedIdx];
    if (*pPlayerGold < item->price) return;        

    // 실제 구매 로직은 인벤토리 시스템으로
    *pPlayerGold -= item->price;
    //inv->AddGold(-item->price);
    inv->AddGold(*pPlayerGold);
    inv->AddItem(item, 1);
    RefreshGold();
    inv->SaveToJson("data/player_inventory.json");
}
void ShopUI::OnSellClicked()
{
    /* 선택 판매 등 구현 예정 */
    std::cout << "판매" << std::endl;
}
