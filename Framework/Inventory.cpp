#include "stdafx.h"
#include "Inventory.h"

using json = nlohmann::json;

bool Inventory::LoadFromJson(const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open()) return false;
    ItemDB::Instance().LoadFromJson("data/shop.json");

    json j; in >> j;
    gold = j.value("gold", 0);

    slots.clear();
    for (const auto& obj : j["slots"])
    {
        int id = obj["id"].get<int>();
        int count = obj["count"].get<int>();
        if (const ItemData* data = ItemDB::Instance().GetItem(id))
            slots.push_back({ data, std::max(1, count) });
    }
    return true;
}

bool Inventory::SaveToJson(const std::string& path) const
{
    json j;
    j["gold"] = gold;
    for (auto& s : slots)
        j["slots"].push_back({ { "id", s.item->id }, { "count", s.count } });

    std::ofstream out(path);
    if (!out.is_open()) return false;
    out << j.dump(2);
    return true;
}

void Inventory::AddItem(const ItemData* item, int cnt)
{
    if (!item || cnt <= 0) return;

    if (auto* s = FindSlot(item->id))
        s->count += cnt;
    else
        slots.push_back({ item, cnt });
}

bool Inventory::RemoveItem(int itemId, int cnt)
{
    if (cnt <= 0) return false;
    if (auto* s = FindSlot(itemId))
    {
        if (s->count < cnt) return false;
        s->count -= cnt;
        if (s->count == 0) {
            auto it = std::remove_if(slots.begin(), slots.end(),
                [itemId](const Slot& sl)
                { return sl.item->id == itemId; });

            slots.erase(it, slots.end());
        }
        return true;
    }
    return false;
}

int Inventory::GetCount(int itemId) const
{
    for (auto& s : slots)
        if (s.item->id == itemId) return s.count;
    return 0;
}

Inventory::Slot* Inventory::FindSlot(int itemId)
{
    for (auto& s : slots)
        if (s.item->id == itemId) return &s;
    return nullptr;
}
