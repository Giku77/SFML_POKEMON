#pragma once
#include "Button.h" 

struct ItemData
{
    int id = 0;
    std::string name{};   
    int price = 0;  
    std::string desc{};   
    std::string iconTexId{};   
};

class ItemDB
{
public:
    static ItemDB& Instance()
    {
        static ItemDB inst;
        return inst;
    }

    bool LoadFromJson(const std::string& jsonPath)
    {
        std::ifstream in(jsonPath);
        if (!in.is_open()) return false;
        nlohmann::json j; in >> j;

        
        const auto& jItems = j["items"];
        for (const auto& obj : jItems)
        {
            ItemData d;
            d.id = obj["id"].get<int>();
            d.name = obj["name"].get<std::string>();
            d.price = obj["price"].get<int>();
            d.desc = obj.value("desc", "");
            d.iconTexId = obj.value("icon", "");
            items[d.id] = std::move(d);
        }

   
        const auto& jShops = j["shops"];
        for (auto it = jShops.begin(); it != jShops.end(); ++it)
        {
            const std::string tag = it.key();
            const auto& arr = it.value();
            std::vector<const ItemData*> list;
            for (int itemId : arr) {
                auto f = items.find(itemId);
                if (f != items.end()) list.push_back(&f->second);
            }
            shopTables[tag] = std::move(list);
        }
        return true;
    }

 
    const ItemData* GetItem(int id) const
    {
        auto f = items.find(id);
        return (f == items.end()) ? nullptr : &f->second;
    }

 
    const std::vector<const ItemData*>& GetShopItems(const std::string& shopTag) const
    {
        static const std::vector<const ItemData*> empty;
        auto f = shopTables.find(shopTag);
        return (f == shopTables.end()) ? empty : f->second;
    }

private:
    std::unordered_map<int, ItemData> items;
    std::unordered_map<std::string, std::vector<const ItemData*>> shopTables;

    ItemDB() = default;
    ~ItemDB() = default;
    ItemDB(const ItemDB&) = delete;
    ItemDB& operator=(const ItemDB&) = delete;
};

class ItemSlot : public Button
{
public:
    ItemSlot(const std::string& name = "ItemSlot")
        : Button(name, true)  
    {
        SetSize({ 180.f, 32.f });   
        ButtonSetFillColor(sf::Color(0, 0, 0, 0));              
        SetOutlineColor(sf::Color::White);
        SetOutlineThickness(1.f);
    }

    void SetItem(const ItemData* data)
    {
        assert(data);
        item = data;

        if (!data->iconTexId.empty()) {
            bg.setTexture(TEXTURE_MGR.Get(data->iconTexId));
            bg.setScale(0.75f, 0.75f);
            bg.setPosition(position + sf::Vector2f{ 2.f, 2.f });
        }

        std::ostringstream oss;
        oss << data->name << "  $" << data->price;
        SetString(oss.str());
        SetCharacterSize(14);
        TextSetFillColor(sf::Color::White);
        TextSetPosition(position + sf::Vector2f{ 38.f, 4.f });

        SetOnClick([this]() {
            if (onSelect) onSelect(item);
            });
    }

    void SetSelected(bool sel)
    {
        ButtonSetFillColor(sel ? sf::Color(80, 120, 255, 200)
            : sf::Color(30, 30, 30, 160));
    }

    void SetOnSelect(std::function<void()> fn) { SetOnClick(std::move(fn)); }

    std::function<void(const ItemData*)> onSelect;

    void SetPosition(const sf::Vector2f& pos) override
    {
        Button::SetPosition(pos);
        if (item && !item->iconTexId.empty())
            bg.setPosition(position + sf::Vector2f{ 2.f, 2.f });
        TextSetPosition(position + sf::Vector2f{ 38.f, 4.f });
    }

    void Draw(sf::RenderWindow& window) override
    {
        if (!active) return;
        if (!item)   return;
        if (isBg) window.draw(bg);
        Button::Draw(window);              // draws rectangle + text
    }

private:
    const ItemData* item = nullptr;
};


