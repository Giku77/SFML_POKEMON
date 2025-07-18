#pragma once
#include "Animator.h"

class AniPlayer : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;
	std::string currentClip = "animations/player_idle.csv";

	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f prevPos = { 0.f, 0.f };
	bool isGrounded = true;
	float speed = 300.f;
	bool isRight = true;

public:
	AniPlayer(const std::string& name = "");
	~AniPlayer() = default;

	void SetCurrentClip(const std::string& s) { currentClip = s; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	const sf::Vector2f& getPrevPos() const { return prevPos; }
	void setPrevPos(const sf::Vector2f& p) { prevPos = p; }

	const Animator& getAnimator() const { return animator; }
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
