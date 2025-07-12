#pragma once
class SpriteAnimator {
private:
    sf::Sprite* target;                
    std::vector<sf::IntRect> frames;   
    float interval;                   
    float timer = 0.f;
    size_t index = 0;
    bool playing = false;             

public:
    SpriteAnimator(sf::Sprite* spr,
        const std::vector<sf::IntRect>& rects,
        float speed = 0.1f)
        : target(spr), frames(rects), interval(speed) {
        if (!frames.empty())
            target->setTextureRect(frames[0]);
    }

    void Update(float dt, bool hover) {
        if (!hover) {       
            index = 0;
            playing = false;
            target->setTextureRect(frames[0]);
            return;
        }

        playing = true;

        timer += dt;
        while (playing && timer >= interval) {
            timer -= interval;
            index = (index + 1) % frames.size();   // ¼øÈ¯
            target->setTextureRect(frames[index]);
        }
    }
};

