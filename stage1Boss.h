//=============================================================================
//
// ステージ1ボスヘッダ [Stage1Boss.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _STAGE1BOSS_H_
#define _STAGE1BOSS_H_


// マクロ定義
#define STAGE1BOSS_TEXTURE_NAME	_T("data/TEXTURE/Enemy/boss1.png")			// ステージ1ボスのテクスチャ
#define STAGE1BOSS_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/boss1Shadow.png")

#define STAGE1BOSS_TEXTURE_SIZE_X					(336)					// テクスチャサイズX
#define STAGE1BOSS_TEXTURE_SIZE_Y					(196)					// テクスチャサイズY

#define STAGE1BOSS_TEXTURE_DIVIDE_X					(2)						// アニメパターンのテクスチャ内分割数（X)
#define STAGE1BOSS_TEXTURE_DIVIDE_Y					(2)						// アニメパターンのテクスチャ内分割数（Y)
#define STAGE1BOSS_ANIM_NUM							(2)						// アニメーションパターン数
#define STAGE1BOSS_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define STAGE1BOSS_INITPOS_X						(-100)					//ステージ1ボスの初期X座標
#define STAGE1BOSS_INITPOS_Y						(-100)					//ステージ1ボスの初期Y座標

//ステージ1ボスのバウンディングボックスの定義
#define STAGE1BOSS_BB_TOP						(-30.0f)
#define STAGE1BOSS_BB_LEFT						(0.0f)
#define STAGE1BOSS_BB_WIDTH						(330.0f)
#define STAGE1BOSS_BB_HIGHT						(70.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー
	int						cntFrame;						//カウントフレーム

	float					radius;							// ポリゴンの半径
	float					angle;							// ポリゴンの角度

	int						form;							//形態
	int						state;							//状態
	int						nextState;						//次に遷移する状態

	int						hp;								//HP
	D3DXVECTOR3				targetPos;						//目標座標
	int						cntMove;						//ムーブカウント
	int						atkNum;							//攻撃回数

	BOSSGAUGE				*gauge;							//ボスゲージ
}STAGE1BOSS;

enum STAGE1BOSS_STATE
{
	STAGE1BOSS_INIT,		//初期状態
	STAGE1BOSS_INTERBAL,	//攻撃後のインターバル
	STAGE1BOSS_WAY1,		//WAY1攻撃
	STAGE1BOSS_WIP1,		//WIP攻撃
	STAGE1BOSS_WIP2,
	STAGE1BOSS_CIRCLE,		//CIRCLE攻撃
	STAGE1BOSS_DESTROYED,	//撃墜状態
	STAGE1BOSS_STATEMAX		
};

enum STAGE1BOSS_FORM
{
	STAGE1BOSS_FIRST,		//第1形態
	STAGE1BOSS_SECOND,		//第2形態
	STAGE1BOSS_FORMMAX
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStage1Boss(int num);							//ステージ1ボス初期化処理
void UninitStage1Boss(void);								//ステージ1ボス終了処理
void UpdateStage1Boss(void);								//ステージ1ボス更新処理
void DrawStage1Boss(void);									//ステージ1ボス描画処理
STAGE1BOSS *GetStage1BossAdr(void);							//ステージ1ボスアドレス取得関数
void SetStage1Boss(D3DXVECTOR3 pos);						//ステージ1ボスのセット関数

#endif
