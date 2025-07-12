#pragma once
#include "stdafx.h"
#include <codecvt>
class MessageMgr {
private:
    std::unordered_map<std::string, std::wstring> messages;
    nlohmann::json j;

public:
    bool Load(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) return false;

        in >> j;

        for (auto it = j.begin(); it != j.end(); ++it) {
            if (!it->is_string())         
                continue;

            std::string key = it.key();
            std::string utf8 = it.value().get<std::string>();

            // 콘솔 디버그용: UTF-8 → UTF-16 변환 후 출력
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wstr = converter.from_bytes(utf8);

            messages[key] = wstr; 
        }

        return true;
    }

    const std::wstring& Get(const std::string& key) const {
        static std::wstring empty = L"[없음]";
        auto it = messages.find(key);
        return (it != messages.end()) ? it->second : empty;
    }

    std::vector<std::wstring> GetArray(const std::string& key) const {
        std::vector<std::wstring> result;

        auto it = j.find(key);
        if (it == j.end() || !it->is_array()) return result;

        for (const auto& val : *it) {
            std::string utf8 = val.get<std::string>();
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wstr = converter.from_bytes(utf8);
  
            result.push_back(wstr);
        }

        return result;
    }
};

