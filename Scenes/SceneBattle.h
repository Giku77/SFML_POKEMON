#pragma once
#include "Scene.h"
#include "BattleMgr.h"

class Text;
class Button;
class SpriteAnimator;

class SceneBattle : public Scene
{
protected:
	sf::Texture tex;
	sf::Sprite sprite;

	Text* pokeName1 = nullptr;
	Text* pokeName2 = nullptr;
	Text* pokeLv1 = nullptr;
	Text* pokeLv2 = nullptr;

	sf::RectangleShape pokeHp1;
	sf::RectangleShape pokeBackHp1;
	sf::RectangleShape pokeHp2;
	sf::RectangleShape pokeBackHp2;

	Text* battleMsg = nullptr;

	Button* mov1 = nullptr;
	Button* mov2 = nullptr;
	Button* mov3 = nullptr;
	Button* mov4 = nullptr;

	Pokemon* ePoke = nullptr;
	int eHp = 0;
	Pokemon* mPoke = nullptr;
	int mHp = 0;


	sf::Sprite pokSprite1;
	sf::Texture pokTex1;
	SpriteAnimator* poke1 = nullptr;
	sf::Sprite pokSprite2;
	sf::Texture pokTex2;
	SpriteAnimator* poke2 = nullptr;

	float overTime = 0.f;

	std::vector<int> pokemonFrameCounts = {
	3, 3, 6,  // 001~003 �̻��ؾ� ~ �̻��ز�
	3, 3, 3,  // 004~006 ���̸� ~ ���ڸ�
	3, 3, 3,  // 007~009 ���α� ~ �źϿ�
	3, 3, 6,  // 010~012 ĳ���� ~ ������
	3, 3, 3,  // 013~015 ������ ~ ��ħ��
	3, 3, 3,  // 016~018 ���� ~ ������
	6, 6,     // 019~020 ���� ~ ��Ʈ��
	3, 3,     // 021~022 ������ ~ ����帱��
	3, 3,     // 023~024 �ƺ� ~ �ƺ�ũ
	6, 6,     // 025~026 ��ī��, ������ (�ϼ� ���� ����)
	3, 3,     // 027~028 �𷡵��� ~ ���� (�ϼ� ���� ����)
	3, 3, 3,  // 029~031 �ϵ己�� ~ �ϵ���
	3, 3, 3,  // 032~034 �ϵ己�� ~ �ϵ�ŷ
	3, 3,     // 035~036 �߻� ~ �Ƚ�
	3, 3,     // 037~038 �Ľ����� ~ ��������
	3, 3,     // 039~040 Ǫ�� ~ Ǫũ��
	6, 6,     // 041~042 �ֹ� ~ ���
	3, 6,     // 043~044 �ѹ��� ~ ������
	6, 3,     // 045~046 ���÷��þ� ~ �Ķ�
	3, 3,     // 047~048 �Ķ�Ʈ ~ ����
	3, 3,     // 049~050 ������ ~ ��״�
	3, 3,     // 051~052 ��Ʈ���� ~ ����
	3, 3,     // 053~054 �丣�ÿ� ~ ����Ĵ�
	3, 3,     // 055~056 ��� ~ ��Ű
	3, 3,     // 057~058 ������ ~ ����
	3,        // 059 ����
	3, 3, 3,  // 060~062 ��ì�� ~ ��ì��
	3, 6, 6,  // 063~065 ĳ�̽� ~ �ĵ�
	3, 3, 3,  // 066~068 ����� ~ ���¸�
	3, 3, 3,  // 069~071 ����� ~ ������Ʈ
	3, 3,     // 072~073 �մ��� ~ ���ĸ�
	3, 3, 3,  // 074~076 ������ ~ ������
	3, 3,     // 077~078 ����Ÿ ~ ���߸�
	3, 3,     // 079~080 �ߵ� ~ �ߵ���
	3, 3,     // 081~082 ���� ~ ��������
	3,        // 083 �Ŀ���
	6, 6,     // 084~085 �ε� ~ ��Ʈ����
	3, 3,     // 086~087 ���� ~ �극��
	3, 3,     // 088~089 ������ ~ ������
	3, 3,     // 090~091 ���� ~ �ĸ���
	3, 3, 3,  // 092~094 ����� ~ ����
	3,        // 095 �ս���
	3, 6,     // 096~097 ������ ~ ������
	3, 3,     // 098~099 ũ�� ~ ŷũ��
	3,        // 100 �����
	3,        // 101 �պ�
	3, 3,     // 102~103 �ƶ� ~ ����
	3, 3,     // 104~105 ������ ~ �ֱ���
	3, 3,     // 106~107 �ö�Ҹ� ~ ȫ����
	3,        // 108 �����
	3, 3,     // 109~110 �ǰ��� ~ �ǵ�����
	6, 6,     // 111~112 ��ī�� ~ �ڻѸ�
	3,        // 113 ��Ű
	3,        // 114 ����
	3,        // 115 Ļī
	3, 3,     // 116~117 ���� ~ �õ��
	6, 6,     // 118~119 ��ġ ~ ����ġ
	3, 3,     // 120~121 �����縮 ~ ����Ÿ
	3,        // 122 ���Ӹ�
	6,        // 123 ����ũ
	3,        // 124 ���ֶ�
	3,        // 125 ������
	3,        // 126 ���׸�
	3,        // 127 �ڻ�����
	3,        // 128 ��Ÿ�ν�
	6, 6,     // 129~130 �׾�ŷ ~ ���󵵽�
	3,        // 131 ������
	3,        // 132 ��Ÿ��
	3, 3, 3, 3,  // 133~135 �̺��� ~ �ν���
	3,        // 136 ������
	3, 3,     // 137~138 �ϳ���Ʈ ~ �Ͻ�Ÿ
	3, 3,     // 139~140 ���� ~ ����Ǫ��
	3,        // 141 ���׶�
	3,        // 142 �Ḹ��
	3, 3, 3,  // 143~145 ������, ���, ���̾�
	3, 3, 3,  // 146~148 �̴� ~ ������
	3,        // 149 ����
	3         // 150 ��
	};
	std::unordered_map<int, Pokemon> ar;
	UiMgr uiMgrBattle;
	PokemonManager pmgr;
	BattleMgr bmgr;

public:
	SceneBattle();
	~SceneBattle() override;

	void CreateMoveAndPokemon();

	void Init() override;
	void Enter() override;

	//void AddPokemons(const SpriteAnimator* p1, const SpriteAnimator* p2);

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

