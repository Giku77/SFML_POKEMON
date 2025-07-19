#include "stdafx.h"
#include "BattleMgr.h"
#include "Text.h"

void BattleMgr::Init(PlayerPokemon* player, EnemyPokemon* enemy)
{
	if (!player || !enemy) throw std::invalid_argument("nullptr pokemon");
	this->player = player;
	this->enemy = enemy;
	currentTurn = Turn::Player;
	IsBattleOver = false;
}

void BattleMgr::Update(float dt)
{
	if (IsBattleOver) return;

	// Enemy 턴이면 자동 행동
	if (currentTurn == Turn::Enemy) {
		turnDelay += dt;
		if (turnDelay > 2.f) {
			EnemyAttack();
			turnDelay = 0.f;
		}
	}
	if (isAddPoke) {
		PokeTime += dt;
		if (PokeTime > 2.f) {
			if (isGetPoke) {
				text->SetString(L"신난다! " + enemy->name + L" 을 잡았다!!");
				isOver = true;
				isGetPoke = false;
				PokeTime = 0.f;
				isAddPoke = false;
			}
			else { 
				text->SetString(L"아쉽다..! 다 잡았는데.."); 
				currentTurn = Turn::Enemy;
				PokeTime = 0.f;
				isAddPoke = false;
			}
		}
	}
	if (isOver) {
		changeTime += dt;
		if (changeTime > 2.f) {
			isOver = false;
			SCENE_MGR.ChangeScene(SceneIds::Game);
		}
	}
}

void BattleMgr::UseMove(int moveIndex)
{
	if (IsBattleOver || currentTurn != Turn::Player) return;
	PlayerAttack(moveIndex);
}

void BattleMgr::PlayerAttack(int moveIndex)
{
	if (moveIndex < 0) return;  
	const MoveData* move = MoveDB::Instance().GetMove(moveIndex);
	int dmg = CalcDamage(player, enemy, move);
	enemy->TakeDamage(dmg);
	text->SetString(sf::String(player->name) + L"의 " + move->kname + "!!");
	std::cout << "적 포켓몬 체력 : " << player->hp << std::endl;

	CheckBattleEnd();
	if (!IsBattleOver) currentTurn = Turn::Enemy;
}

void BattleMgr::EnemyAttack()
{
	int idx = ChooseEnemyMove();
	const MoveData* move = MoveDB::Instance().GetMove(enemy->moves[idx].id);
	int dmg = CalcDamage(enemy, player, move);
	player->TakeDamage(dmg);
	text->SetString(sf::String(enemy->name) + L"의 " + move->kname + "!!");
	std::cout << "플레이어 포켓몬 체력 : " << enemy->hp << std::endl;

	CheckBattleEnd();
	if (!IsBattleOver) currentTurn = Turn::Player;
}

void BattleMgr::CheckBattleEnd()
{
	if (player->hp == 0 || enemy->hp == 0) {
		if (player->hp == 0) text->SetString(sf::String(player->name) + L" 는 쓰러졌다.");
		if (enemy->hp == 0) {
			int rExp = Utils::RandomRange(10, 50);
			player->experience += rExp;
			if (player->experience >= 100) {
				int xExp = player->experience - 100;
				player->level++;
				player->experience = xExp;
			}

			text->SetString(sf::String(enemy->name) + L" 는 쓰러졌다.\n" + std::to_wstring(rExp) + L"의 경험치를 얻었다.");
		}
		IsBattleOver = true;
	}
}

int BattleMgr::CalcDamage(const Pokemon* atk, const Pokemon* def, const MoveData* m) const
{
	const int level = atk->level;
	const int attack = atk->attack;
	const int defend = def->defense;
    //const int level = 5;
	//const int attack = 10;
	//const int defend = 30;
	int dmg = (((2 * level / 5 + 2) * m->power * attack / std::max(1, defend)) / 50) + 2;
	return std::max(1, dmg);
}

int BattleMgr::ChooseEnemyMove() const
{
	std::vector<int> usable;
	for (int i = 0; i < enemy->getMoveSize(); ++i) {
		const MoveData* move = MoveDB::Instance().GetMove(enemy->moves[i].id);
		if (move->pp > 0) usable.push_back(i);
	}

	if (usable.empty()) return 0; // Struggle 대용

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, (int)usable.size() - 1);
	return usable[dist(gen)];
}

void BattleMgr::getAddPokemon(const Pokemon& p, Inventory& u)
{
	u.LoadFromJson("data/player_inventory.json");
	if (u.RemoveItem(3, 1)) {
		text->SetString(L"가라!! 몬스터볼!");
		if (Utils::RandomRange(0.f, 1.f) < 0.5f) {
			isGetPoke = true;
			PokemonManager pm;
			//pm.LoadGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
			/*for (auto& p : pm.GetAll()) {
				pm.AddPokemon(p.second);
			}*/
			pm.AddPokemon(*enemy);
			pm.SaveGame(InputMgr::GetinputBuffer(), "data/player_pokemon.json");
		}
		isAddPoke = true;
		u.SaveToJson("data/player_inventory.json");
	}
	else text->SetString(L"몬스터볼이 없다!");
}

