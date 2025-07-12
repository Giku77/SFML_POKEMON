#pragma once
#include "MessageMgr.h"

class StoryPlayer
{
private:
	std::vector<std::wstring> script;
	size_t index = 0;
	bool finished = false;


public:
	void Load(const std::string& key, const MessageMgr& mgr) {
		script.clear();
		index = 0;
		finished = false;

		auto arr = mgr.GetArray(key);
		script.insert(script.end(), arr.begin(), arr.end());

	}

	const size_t& getIndex() const { return index; }

	const std::wstring& GetCurrent() const {
		static std::wstring empty = L"";
		if (index < script.size()) return script[index];
		return empty;
	}

	const std::wstring& Next() {
		if (index < script.size()) {
			return script[index++];
		}
		else {
			finished = true;
			static std::wstring empty = L"";
			return empty;
		}
	}

	bool IsFinished() const { return finished; }
};

