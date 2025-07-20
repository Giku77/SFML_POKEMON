#include "stdafx.h"
struct BattleTransition {
    enum class Phase { Flash, Circle, Done };
    Phase phase = Phase::Flash;

    sf::RectangleShape flash;   // 화면 전체 흰 사각
    sf::CircleShape    circle;  // 화면 중심 원

    float timer = 0.f;

    BattleTransition(sf::Vector2u winSize, int n, const sf::View& uiView) {
        // 1) 처음 사각플래시 (불투명→투명)
        flash.setSize(sf::Vector2f(winSize));
        flash.setFillColor(sf::Color::White);

        // 2) 원 : 초기 반지름 0, 색 흰색
        circle.setRadius(0.f);
        circle.setFillColor(sf::Color::Black);
        circle.setOrigin(0.f, 0.f);               // 이후 scale 로 키움
        sf::Vector2i centerPx{ int(winSize.x / 2), int(winSize.y / 2) };
        sf::Vector2f centerUi =
            FRAMEWORK.GetWindow().mapPixelToCoords(centerPx, uiView);
        circle.setPosition(centerUi);
        circle.setScale(0.f, 0.f);
        phase = static_cast<Phase>(n);
    }

    /// dt 만큼 진행, true 반환 시 애니메이션 완료
    bool update(float dt) {
        timer += dt;

        switch (phase) {
        case Phase::Flash: {           // 0 ~ 0.2 s
            float alpha = 255.f * (1.f - timer / 0.2f);  // 255 → 0
            flash.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
            if (timer >= 0.2f) { timer = 0.f; phase = Phase::Circle; }
            break;
        }
        case Phase::Circle: {          // 0 ~ 0.4 s
            float t = timer / 0.4f;    // 0 → 1
            float s = 1.f + t * 8.f;     // scale 1 → 9
            circle.setScale(s, s);
            if (timer >= 0.4f) phase = Phase::Done;
            break;
        }
        case Phase::Done:
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& w) {
        if (phase == Phase::Flash)
            w.draw(flash, sf::BlendAdd);   // Additive로 번쩍
        else if (phase == Phase::Circle) {
            w.draw(circle);
        }
    }
};

struct SpinCover {
    sf::RectangleShape rect;
    float timer = 0.f;
    bool done = false;

    SpinCover(sf::Vector2u win, const sf::View& uiView) {
        // 대각선보다 길게 만들어 두면 회전해도 화면 전체를 덮음
        float diag = std::sqrtf(win.x * win.x + win.y * win.y);
        rect.setSize({ diag, diag });
        rect.setOrigin(diag / 2.f, diag / 2.f);
        sf::Vector2f u2f =
            FRAMEWORK.GetWindow().mapPixelToCoords(sf::Vector2i(win.x, 0), uiView);
        rect.setPosition(u2f);          // 오른쪽 위 모서리에서 시작
        rect.setFillColor(sf::Color::Black);
        rect.setRotation(0.f);
    }
    bool update(float dt) {
        timer += dt;
        rect.setRotation(-720.f * timer);  // 2초 동안 두 바퀴 회전
        rect.setPosition(rect.getPosition().x - 600.f * dt,   // 왼쪽 아래로 이동
            rect.getPosition().y + 300.f * dt);
        if (timer >= 2.f) { done = true; }
        return done;
    }
    void draw(sf::RenderWindow& w) { w.draw(rect); }
};

struct SlashWipe {
    sf::RectangleShape left, right;
    float timer = 0.f;
    bool done = false;
    SlashWipe(sf::Vector2u win, const sf::View& uiView) {
        float halfW = static_cast<float>(win.x) * 0.5f;
        float fullH = static_cast<float>(win.y);
        left.setSize({ halfW, fullH });
        right.setSize({ halfW, fullH });
        sf::Vector2f lef =
            FRAMEWORK.GetWindow().mapPixelToCoords(sf::Vector2i(-halfW, 0), uiView);
        sf::Vector2f rig =
            FRAMEWORK.GetWindow().mapPixelToCoords(sf::Vector2i(win.x, 0), uiView);
        left.setPosition(lef);        // 화면 밖 왼쪽
        right.setPosition(rig);            // 화면 밖 오른쪽
        left.setFillColor(sf::Color::Black);
        right.setFillColor(sf::Color::Black);
    }
    bool update(float dt) {
        timer += dt;
        float t = Utils::Clamp(timer / 0.5f, 0.f, 1.f);     // 0~0.5초
        left.setPosition(-left.getSize().x * (1.f - t), 0);
        right.setPosition(right.getSize().x * (1.f - t) + left.getSize().x, 0);
        if (timer >= 0.5f) done = true;
        return done;
    }
    void draw(sf::RenderWindow& w) { w.draw(left); w.draw(right); }
};