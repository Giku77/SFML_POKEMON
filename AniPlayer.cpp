#include "stdafx.h"
#include "AniPlayer.h"

AniPlayer::AniPlayer(const std::string& name)
	: GameObject(name)
{
}

void AniPlayer::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void AniPlayer::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void AniPlayer::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void AniPlayer::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(body, originPreset);
	}
}

void AniPlayer::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = Utils::SetOrigin(body, originPreset);
}

void AniPlayer::Init()
{
	animator.SetTarget(&body);

	animator.AddEvent("Idle", 0,
		[]()
		{
			//std::cout << "!!" << std::endl;
		}
	);

	animator.AddEvent("Idle", 0,
		[]()
		{
			//std::cout << "??" << std::endl;
		}
	);
}

void AniPlayer::Release()
{
}

void AniPlayer::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetPosition({ 500.f, 1100.f });
	SetScale({ 0.5f, 0.5f });

	animator.Play("animations/player_idle.csv");
	SetOrigin(Origins::BC);
}

void AniPlayer::Update(float dt)
{
	animator.Update(dt);

	sf::Vector2f dir = InputMgr::GetPriorityDirection();

	if (isGrounded)
	{
		velocity = dir * speed;
	}
	if (!isGrounded)
	{
		velocity += gravity * dt;
	}
	position += velocity * dt;
	SetPosition(position);

	if (dir.x != 0.f)
	{
		SetScale({ 0.5f * (dir.x > 0.f ? 1.f : -1.f), 0.5f });
	}

	std::string current = animator.GetCurrentClipId();
	if (dir.x != 0.f && current != "Run")
	{
		animator.Play("animations/player_run.csv");
	}
	else if (dir.y < 0.f && current != "Up")
	{
		animator.Play("animations/player_Up.csv");
	}
	else if (dir.y > 0.f && current != "Down")
	{
		animator.Play("animations/player_Down.csv");
	}
	else if (dir == sf::Vector2f(0.f, 0.f) &&
		(current == "Run" || current == "Up" || current == "Down"))
	{
		animator.Play("animations/player_idle.csv");
	}
}

void AniPlayer::Draw(sf::RenderWindow& window)
{
	//std::cout << "[AniPlayer] Draw: " << position.x << ", " << position.y << std::endl;
	window.draw(body);
}
