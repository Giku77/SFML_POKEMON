#pragma once
class SpriteAnimator {
private:
    sf::Sprite* target;                
    std::vector<sf::IntRect> frames;   
    float interval;                   
    float timer = 0.f;
    size_t index = 0;
    bool playing = false;      
    bool isRemoveBg = false;

public:
    SpriteAnimator(sf::Sprite* spr,
        const std::vector<sf::IntRect>& rects,
        float speed = 0.1f, bool b = false)
        : target(spr), frames(rects), interval(speed), isRemoveBg(b) {
        if (!frames.empty())
            target->setTextureRect(frames[0]);
    }
    SpriteAnimator(sf::Sprite* spr,
        const sf::Texture& tex,
        int frameWidth,
        int frameHeight,
        float speed = 0.1f, bool b = false)
        : target(spr), interval(speed), isRemoveBg(b)
    {
        int texWidth = tex.getSize().x;
        int texHeight = tex.getSize().y;

        int cols = texWidth / frameWidth;
        int rows = texHeight / frameHeight;

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::IntRect rect(x * frameWidth, y * frameHeight, frameWidth, frameHeight);
                frames.push_back(rect);
            }
        }

        if (!frames.empty())
            target->setTextureRect(frames[0]);
    }
    void setPlaying(bool b) { playing = b; }
    const bool& getPlaying() const { return playing; }

    void setIndex(int i) { index = i; }

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

