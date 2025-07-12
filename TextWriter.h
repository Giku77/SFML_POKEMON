#pragma once
class TextWriter {
private:
    sf::Text* targetText;
    std::wstring fullText;
    std::wstring currentText;

    float interval;      // 글자 간 시간
    float timer = 0.f;
    size_t index = 0;
    bool isDone = false;

public:
    TextWriter(sf::Text* text, const std::wstring& message, float speed = 0.05f)
        : targetText(text), fullText(message), interval(speed) {
        targetText->setString(L"");
    }

    void Update(float dt) {
        if (isDone) return;

        timer += dt;
        while (timer >= interval && index < fullText.size()) {
            currentText += fullText[index++];
            targetText->setString(currentText);
            timer -= interval;
        }

        if (index >= fullText.size())
            isDone = true;
    }

    bool IsFinished() const { return isDone; }

    void Skip() {
        targetText->setString(fullText);
        isDone = true;
    }

    void Reset() {
        currentText.clear();
        index = 0;
        timer = 0.f;
        isDone = false;
        targetText->setString(L"");
    }
};
