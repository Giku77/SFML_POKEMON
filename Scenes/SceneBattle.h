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
	3, 3, 6,  // 001~003 이상해씨 ~ 이상해꽃
	3, 3, 3,  // 004~006 파이리 ~ 리자몽
	3, 3, 3,  // 007~009 꼬부기 ~ 거북왕
	3, 3, 6,  // 010~012 캐터피 ~ 버터플
	3, 3, 3,  // 013~015 뿔충이 ~ 독침붕
	3, 3, 3,  // 016~018 구구 ~ 피죤투
	6, 6,     // 019~020 꼬렛 ~ 레트라
	3, 3,     // 021~022 깨비참 ~ 깨비드릴조
	3, 3,     // 023~024 아보 ~ 아보크
	6, 6,     // 025~026 피카츄, 라이츄 (암수 구분 있음)
	3, 3,     // 027~028 모래두지 ~ 고지 (암수 구분 있음)
	3, 3, 3,  // 029~031 니드런♀ ~ 니드퀸
	3, 3, 3,  // 032~034 니드런♂ ~ 니드킹
	3, 3,     // 035~036 삐삐 ~ 픽시
	3, 3,     // 037~038 식스테일 ~ 나인테일
	3, 3,     // 039~040 푸린 ~ 푸크린
	6, 6,     // 041~042 주뱃 ~ 골뱃
	3, 6,     // 043~044 뚜벅쵸 ~ 냄새꼬
	6, 3,     // 045~046 라플레시아 ~ 파라스
	3, 3,     // 047~048 파라섹트 ~ 콘팡
	3, 3,     // 049~050 도나리 ~ 디그다
	3, 3,     // 051~052 닥트리오 ~ 나옹
	3, 3,     // 053~054 페르시온 ~ 고라파덕
	3, 3,     // 055~056 골덕 ~ 망키
	3, 3,     // 057~058 성원숭 ~ 가디
	3,        // 059 윈디
	3, 3, 3,  // 060~062 발챙이 ~ 강챙이
	3, 6, 6,  // 063~065 캐이시 ~ 후딘
	3, 3, 3,  // 066~068 알통몬 ~ 괴력몬
	3, 3, 3,  // 069~071 모다피 ~ 우츠보트
	3, 3,     // 072~073 왕눈해 ~ 독파리
	3, 3, 3,  // 074~076 꼬마돌 ~ 딱구리
	3, 3,     // 077~078 포니타 ~ 날쌩마
	3, 3,     // 079~080 야돈 ~ 야도란
	3, 3,     // 081~082 코일 ~ 레어코일
	3,        // 083 파오리
	6, 6,     // 084~085 두두 ~ 두트리오
	3, 3,     // 086~087 쥬쥬 ~ 쥬레곤
	3, 3,     // 088~089 질퍽이 ~ 질뻐기
	3, 3,     // 090~091 셀러 ~ 파르셀
	3, 3, 3,  // 092~094 고오스 ~ 팬텀
	3,        // 095 롱스톤
	3, 6,     // 096~097 슬리프 ~ 슬리퍼
	3, 3,     // 098~099 크랩 ~ 킹크랩
	3,        // 100 찌리리공
	3,        // 101 붐볼
	3, 3,     // 102~103 아라리 ~ 나시
	3, 3,     // 104~105 탕구리 ~ 텅구리
	3, 3,     // 106~107 시라소몬 ~ 홍수몬
	3,        // 108 내루미
	3, 3,     // 109~110 또가스 ~ 또도가스
	6, 6,     // 111~112 뿔카노 ~ 코뿌리
	3,        // 113 럭키
	3,        // 114 덩쿠리
	3,        // 115 캥카
	3, 3,     // 116~117 쏘드라 ~ 시드라
	6, 6,     // 118~119 콘치 ~ 왕콘치
	3, 3,     // 120~121 별가사리 ~ 아쿠스타
	3,        // 122 마임맨
	6,        // 123 스라크
	3,        // 124 루주라
	3,        // 125 에레브
	3,        // 126 마그마
	3,        // 127 쁘사이저
	3,        // 128 켄타로스
	6, 6,     // 129~130 잉어킹 ~ 갸라도스
	3,        // 131 라프라스
	3,        // 132 메타몽
	3, 3, 3, 3,  // 133~135 이브이 ~ 부스터
	3,        // 136 폴리곤
	3, 3,     // 137~138 암나이트 ~ 암스타
	3, 3,     // 139~140 투구 ~ 투구푸스
	3,        // 141 프테라
	3,        // 142 잠만보
	3, 3, 3,  // 143~145 프리져, 썬더, 파이어
	3, 3, 3,  // 146~148 미뇽 ~ 망나뇽
	3,        // 149 뮤츠
	3         // 150 뮤
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

