#include "stdafx.h"
#include "ShopUI.h"


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
    panelBg.setTexture(TEXTURE_MGR.Get("ui/shop_panel.png"));
    panelBg.setPosition(PANEL_ORIGIN);

    title.SetFontId("font/pokemon-dppt.ttf");
    title.SetString("Poké Mart");
    title.SetCharacterSize(26);
    title.SetFillColor(sf::Color::Yellow);
    title.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 20.f, 10.f });

    goldText.SetFontId("fonts/pokemon-dppt.ttf");
    goldText.SetCharacterSize(22);
    goldText.SetFillColor(sf::Color::White);
    goldText.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 260.f, 10.f });

    infoText.SetFontId("fonts/pokemon-dppt.ttf");
    infoText.SetCharacterSize(18);
    infoText.SetFillColor(sf::Color(230, 230, 230));
    infoText.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 20.f, 255.f });
    infoText.SetString(" ");                // 초기 공백

    closeBtn.SetButton({ 60.f,28.f }, sf::Color(80, 80, 80, 200), "fonts/pokemon-dppt.ttf");
    closeBtn.SetString("X");
    closeBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ PANEL_WIDTH - 70.f, 5.f });
    closeBtn.SetOnClick([this]() { Close(); });

    buyBtn.SetButton({ 90.f,32.f }, sf::Color(0, 120, 255, 200), "fonts/pokemon-dppt.ttf");
    buyBtn.SetString("Buy");
    buyBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 275.f, 210.f });
    buyBtn.SetOnClick([this]() { OnBuyClicked(); });

    sellBtn.SetButton({ 90.f,32.f }, sf::Color(0, 200, 100, 200), "fonts/pokemon-dppt.ttf");
    sellBtn.SetString("Sell");
    sellBtn.SetPosition(PANEL_ORIGIN + sf::Vector2f{ 275.f, 250.f });
    sellBtn.SetOnClick([this]() { OnSellClicked(); });

    const float rowHeight = 32.f;
    for (int r = 0; r < visibleRows; ++r)
    {
        auto* slot = new ItemSlot("Slot_" + std::to_string(r));
        slot->Init();
        slot->SetPosition(PANEL_ORIGIN + sf::Vector2f{ 20.f, 50.f + r * rowHeight });
        slot->SetOnSelect([this, r]() { OnSlotClicked(r); });
        slots.push_back(slot);
    }
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
    infoText.SetString(" ");
    RefreshSlots();
}


void ShopUI::Open(const std::string& shopTag, int playerGold)
{
    items = ItemDB::Instance().GetShopItems(shopTag);
    pPlayerGold = &playerGold;         
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

    float wheel = InputMgr::GetMouseWheelDelta();
    if (wheel != 0.f && items.size() > visibleRows)
    {
        firstRow = Utils::Clamp(firstRow - wheel, 0,
            int(items.size() - visibleRows));
        RefreshSlots();
    }

    closeBtn.Update(dt);  
    buyBtn.Update(dt);  
    sellBtn.Update(dt);
    for (auto* s : slots) s->Update(dt);
}

void ShopUI::Draw(sf::RenderWindow& window)
{
    if (!active) return;

    window.draw(panelBg);
    title.Draw(window);
    goldText.Draw(window);
    infoText.Draw(window);

    for (auto* s : slots) s->Draw(window);

    buyBtn.Draw(window);
    sellBtn.Draw(window);
    closeBtn.Draw(window);
}

void ShopUI::RefreshGold()
{
    goldText.SetString("₽ " + std::to_string(*pPlayerGold));
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
    if (idx >= (int)items.size()) return;

    selectedIdx = idx;
    RefreshSlots();

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
    RefreshGold();
}
void ShopUI::OnSellClicked()
{
    /* 선택 판매 등 구현 예정 */
}
