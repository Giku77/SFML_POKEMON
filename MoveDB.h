#pragma once
struct MoveData
{
    int id = 0;
    std::string ename;        // 영어 이름
    sf::String kname;        // 한국어 이름
    sf::String type;         // 노말, 불, 물 …
    sf::String category;     // 물리/특수/변화
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
            std::cerr << "JSON 파싱 오류: " << e.what() << '\n';
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
        if (j.is_array())               arr = &j;          // moves_korean_kr_full.json 형태
        else if (j.contains("items"))   arr = &j["items"]; // {"items":[…]} 형태
        else
        {
            std::cerr << "지원되지 않는 JSON 구조\n";
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
            d.kname = converter.from_bytes(obj.value("kname", ""));         // 누락 시 ""
            d.type = converter.from_bytes(obj.value("type", ""));         // ""
            d.category = converter.from_bytes(obj.value("category", ""));
            d.power = getIntSafe(obj, "power", 0);   
            d.accuracy = getIntSafe(obj, "accuracy", 0);   
            d.pp = getIntSafe(obj, "pp", 0);    

            moves[d.id] = std::move(d);
        }
        std::cout << "[Loader] 기술 " << moves.size() << "개 로드\n";
        return true;
    }


private:
    std::unordered_map<int, MoveData> moves;

    MoveDB() = default;
    ~MoveDB() = default;
    MoveDB(const MoveDB&) = delete;
    MoveDB& operator=(const MoveDB&) = delete;
};

