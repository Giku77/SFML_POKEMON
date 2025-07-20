#include "stdafx.h"
#include "InventoryUI.h"
#include "PokemonManager.h"
#include "PokemonDB.h"


sf::Vector2f PANELINV_ORIGIN{ 400.f, 190.f };

InventoryUI::InventoryUI(const std::string& name)
    : UI(name)
{
}

void InventoryUI::Init()
{
    panelTex = Utils::loadWithColorKey("graphics/PokeInv.png", sf::Color(208, 232, 248));
    panelBackground.setTexture(panelTex);
    panelBackground.setTextureRect({ 349, 240, 516 - 349, 381 - 240});
    panelBackground.setScale(3.f, 3.f);
    panelBackground.setPosition(PANELINV_ORIGIN);

    //slots.reserve(rows * cols);
    for (int i = 0; i < 3; ++i)
    {
        auto* slot = new ItemSlot("InvSlot" + std::to_string(i));
        slot->Init();                                   // 폰트 등 준비
        //int r = i / cols, c = i % cols;
        //sf::Vector2f pos = { 100.f + c * cellW, 100.f + r * cellH };
        sf::Vector2f pos = { 250.f, 100.f + cellH * i };
        slot->SetPosition(PANELINV_ORIGIN + pos);
        slot->isMouseOverColor = true;
        slot->ButtonSetFillColor(sf::Color(80, 80, 80));
        slot->TextSetFillColor(sf::Color::Black);
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
    //mgr.Update(dt);
    for (auto* s : slots) s->Update(dt);
    if (SelectIndex != -1) {
        if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
        {
            const auto& invSlots = inventory->GetSlots();
            const auto* data = invSlots[SelectIndex].item;
            int count = invSlots[SelectIndex].count;
            //std::cout << "아이템 선택" << std::endl;
            if (inventory->RemoveItem(data->id, 1)) {
                RefreshSlots();
                PokemonManager mgr;
                mgr.LoadGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
                std::unordered_map<int, Pokemon> MyPokemons = mgr.GetAll();
                switch (data->id)
                {
                case 1:
                    for (auto& p : MyPokemons) {
                        p.second.hp += 20;
                        if(p.second.hp > PokemonDB::Instance().GetPokemon(p.second.id)->hp)
                            p.second.hp = PokemonDB::Instance().GetPokemon(p.second.id)->hp;
                        mgr.AddPokemon(p.second);
                        break;
                    }
                    break;
                case 2:
                    for (auto& p : MyPokemons) {
                        p.second.hp += 60;
                        if (p.second.hp > PokemonDB::Instance().GetPokemon(p.second.id)->hp)
                            p.second.hp = PokemonDB::Instance().GetPokemon(p.second.id)->hp;
                        mgr.AddPokemon(p.second);
                        break;
                    }
                default:
                    break;
                }
                mgr.SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
                inventory->SaveToJson("data/player_inventory.json");
            }
            SelectIndex = -1;
        }
    }
}

void InventoryUI::Draw(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(panelBackground);   // 배경이 있으면
    //mgr.Draw(window);
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

    SelectIndex = visualIdx;

    const auto* data = invSlots[visualIdx].item;
    int count = invSlots[visualIdx].count;

    //std::cout << "아이템 선택" << std::endl;
    std::wcout << L"선택된 아이템 : " << data->name
        << L" (" << count << L"개)\n";
}
