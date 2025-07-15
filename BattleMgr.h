#pragma once
#include "BattlePokemon.h"
class BattleMgr
{
public:
    enum class Turn {
        Player,
        Enemy
    };

    void Init(PlayerPokemon* player, EnemyPokemon* enemy);
    void Update(float dt);
    void UseMove(int moveIndex); // 플레이어가 기술 선택 시 호출
    bool IsBattleOver() const;

    const Pokemon& GetPlayerPokemon() const;
    const Pokemon& GetEnemyPokemon() const;
    Turn GetCurrentTurn() const;

private:
    PlayerPokemon* player;
    EnemyPokemon* enemy;
    Turn currentTurn = Turn::Player;

    void PlayerAttack(int moveIndex);
    void EnemyAttack(); // 간단한 AI
    void CheckBattleEnd();
};

