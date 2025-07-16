#pragma once
#include "BattlePokemon.h"
class Text;
class BattleMgr
{
public:
    enum class Turn {
        Player,
        Enemy
    };

    void Init(PlayerPokemon* player, EnemyPokemon* enemy);
    void SetText(Text* t) { text = t; };
    void Update(float dt);
    void UseMove(int moveIndex); // �÷��̾ ��� ���� �� ȣ��
    bool BattleOver() const { return IsBattleOver; }

    const Pokemon& GetPlayerPokemon() const { return *player; }
    const Pokemon& GetEnemyPokemon() const { return *enemy; }
    Turn GetCurrentTurn() const { return currentTurn; }

private:
    PlayerPokemon* player;
    EnemyPokemon* enemy;
    Text* text = nullptr;
    Turn currentTurn = Turn::Player;

    bool IsBattleOver = false;
    void PlayerAttack(int moveIndex);
    void EnemyAttack(); // ������ AI
    void CheckBattleEnd();
    int CalcDamage(const Pokemon* atk, const Pokemon* def, const MoveData* m) const;
    int ChooseEnemyMove() const;
    void TakeDamage(int d);

};

