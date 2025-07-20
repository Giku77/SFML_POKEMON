#pragma once
#include "BattlePokemon.h"
#include "Inventory.h"

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
    //void SetSprite(sf::Sprite* p, sf::Sprite* pp) { 
    //    p1 = p;
    //    p2 = pp;
    //};
    void Update(float dt);
    void UseMove(int moveIndex); // 플레이어가 기술 선택 시 호출
    bool BattleOver() const { return IsBattleOver; }

    const Pokemon& GetPlayerPokemon() const { return *player; }
    const Pokemon& GetEnemyPokemon() const { return *enemy; }
    Turn GetCurrentTurn() const { return currentTurn; }
    void getAddPokemon(const Pokemon& p, Inventory& u);

private:
    PlayerPokemon* player;
    EnemyPokemon* enemy;
    //sf::Sprite* p1 = nullptr;
    //sf::Sprite* p2 = nullptr;
    Text* text = nullptr;
    Turn currentTurn = Turn::Player;
    float turnDelay = 0.f;

    bool IsBattleOver = false;
    void PlayerAttack(int moveIndex);
    void EnemyAttack(); // 간단한 AI
    void CheckBattleEnd();
    int CalcDamage(const Pokemon* atk, const Pokemon* def, const MoveData* m) const;
    int ChooseEnemyMove() const;

    bool isAddPoke = false;
    bool isGetPoke = false;
    bool isOver = false;
    //bool isEffect = false;
    float PokeTime = 0.f;
    float changeTime = 0.f;
    //float effectTIme = 0.f;
};

