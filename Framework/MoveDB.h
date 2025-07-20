#pragma once
struct MoveData
{
    int id = 0;
    std::string ename;        // ���� �̸�
    sf::String kname;        // �ѱ��� �̸�
    sf::String type;         // �븻, ��, �� ��
    sf::String category;     // ����/Ư��/��ȭ
    int power = 0;
    int accuracy = 0;
    int pp = 0;
};

class MoveDB
{
public:
    static MoveDB& Instance()
    {
        static MoveDB inst;
        return inst;
    }

    const MoveData* GetMove(int id) const
    {
        auto f = moves.find(id);
        return (f == moves.end()) ? nullptr : &f->second;
    }


    bool LoadFromJson(const std::string& jsonPath)
    {
        std::ifstream in(jsonPath);
        if (!in.is_open())
            return false;

        nlohmann::json j;
        try { in >> j; }
        catch (const std::exception& e)
        {
            std::cerr << "JSON �Ľ� ����: " << e.what() << '\n';
            return false;
        }

        auto getIntSafe = [](const nlohmann::json& o,
            const char* key,
            int def = 0) -> int
            {
                if (!o.contains(key) || o[key].is_null())
                    return def;                             
                if (o[key].is_number_integer())
                    return o[key].get<int>();             
                if (o[key].is_string())
                {
                    return def;
                }
                return def;                                
            };

        const nlohmann::json* arr = nullptr;
        if (j.is_array())               arr = &j;          // moves_korean_kr_full.json ����
        else if (j.contains("items"))   arr = &j["items"]; // {"items":[��]} ����
        else
        {
            std::cerr << "�������� �ʴ� JSON ����\n";
            return false;
        }

        for (const auto& obj : *arr)
        {
            if (!obj.contains("id") || !obj.contains("ename"))
                continue;                       // or throw

            MoveData d;
            d.id = obj["id"].get<int>();
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            d.ename = obj["ename"].get<std::string>();
            d.kname = converter.from_bytes(obj.value("kname", ""));         // ���� �� ""
            d.type = converter.from_bytes(obj.value("type", ""));         // ""
            d.category = converter.from_bytes(obj.value("category", ""));
            d.power = getIntSafe(obj, "power", 0);   
            d.accuracy = getIntSafe(obj, "accuracy", 0);   
            d.pp = getIntSafe(obj, "pp", 0);    

            moves[d.id] = std::move(d);
        }
        std::cout << "[Loader] ��� " << moves.size() << "�� �ε�\n";
        return true;
    }


private:
    std::unordered_map<int, MoveData> moves;

    MoveDB() = default;
    ~MoveDB() = default;
    MoveDB(const MoveDB&) = delete;
    MoveDB& operator=(const MoveDB&) = delete;
};

