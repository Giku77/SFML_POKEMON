#pragma once
#include "ItemDB.h"
class Inventory
{
public:
    struct Slot {
        const ItemData* item;  
        int count;            
    };


    bool LoadFromJson(const std::string& path);
    bool SaveToJson(const std::string& path) const;


    void AddItem(const ItemData* item, int cnt = 1);
    bool RemoveItem(int itemId, int cnt = 1);    
    int  GetCount(int itemId) const;


    int GetGold() const { return gold; }
    void AddGold(int n) { gold = n; }
    const std::vector<Slot>& GetSlots() const { return slots; }

private:
    int gold = 0;
    std::vector<Slot> slots;

    Slot* FindSlot(int itemId);                 
};

