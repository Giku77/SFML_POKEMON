#include "stdafx.h"
#include "InventoryUI.h"

InventoryUI::InventoryUI(const std::string& name)
    : UI(name)
{
}

void InventoryUI::Init()
{
    slots.reserve(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
    {
        auto* slot = new ItemSlot("InvSlot" + std::to_string(i));
        slot->Init();                                   // 폰트 등 준비
        int r = i / cols, c = i % cols;
        sf::Vector2f pos = { 100.f + c * cellW, 100.f + r * cellH };
        slot->SetPosition(pos);
        slot->isMouseOverColor = true;
        slot->SetOnSelect([this, i]() { OnSlotClicked(i); });
        slots.push_back(slot);
    }
}

void InventoryUI::Release()
{
    for (auto* s : slots) delete s;
    slots.clear();
}

void InventoryUI::Reset()
{
}

void InventoryUI::Open(Inventory& inv)
{
    inventory = &inv;
    RefreshSlots();
    active = true;
}

void InventoryUI::Update(float dt)
{
    if (!active) return;
    mgr.Update(dt);
    for (auto* s : slots) s->Update(dt);
}

void InventoryUI::Draw(sf::RenderWindow& window)
{
    if (!active) return;
    // window.draw(panelBackground);   // 배경이 있으면
    mgr.Draw(window);
    for (auto* s : slots) s->Draw(window);
}

void InventoryUI::RefreshSlots()
{
    if (!inventory) return;
    const auto& invSlots = inventory->GetSlots();   // vector<Inventory::Slot>

    // 화면 슬롯 수(rows*cols) == 16
    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (i < static_cast<int>(invSlots.size()))
        {
            slots[i]->SetActive(true);
            slots[i]->SetInvItem(invSlots[i].item, invSlots[i].count);
        }
        else
        {
            slots[i]->SetActive(false);
        }
    }
}

void InventoryUI::OnSlotClicked(int visualIdx)
{
    if (!inventory) return;
    const auto& invSlots = inventory->GetSlots();
    if (visualIdx >= static_cast<int>(invSlots.size())) return;

    const auto* data = invSlots[visualIdx].item;
    int count = invSlots[visualIdx].count;

    std::wcout << L"선택된 아이템 : " << data->name
        << L" (" << count << L"개)\n";

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
    {
        if (inventory->RemoveItem(data->id, 1))
            RefreshSlots();                  // 변화 반영
    }
}
