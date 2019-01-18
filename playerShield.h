//=============================================================================
//
// プレイヤーシールドヘッダ [PlayerShield.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYERSHIELD_H_
#define _PLAYERSHIELD_H_


// マクロ定義
#define PLAYERSHIELD_TEXTURE_NAME	_T("data/TEXTURE/player/playerShield.png")	// プレイヤーシールドのテクスチャ

#define PLAYERSHIELD_TEXTURE_SIZE_X					(80)					// テクスチャサイズX
#define PLAYERSHIELD_TEXTURE_SIZE_Y					(80)					// テクスチャサイズY

#define PLAYERSHIELD_INITPOS_X						(-100)						//プレイヤーシールドの初期X座標
#define PLAYERSHIELD_INITPOS_Y						(-100)						//プレイヤーシールドの初期Y座標

//プレイヤーシールドのバウンディングサークルの定義
#define PLAYERSHIELD_BC_RADIUS						(40.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;							// ポリゴンの角度
	float					scale;							// ポリゴンのスケール
	int						cntFrame;
	int						state;

}PLAYERSHIELD;

enum PLAYERSHIELD_COLOR
{
	SHIELD_ORANGE,
	SHIELD_GREEN,
	SHIELD_TYPEMAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerShield(int num);							//プレイヤーシールド初期化処理
void UninitPlayerShield(void);								//プレイヤーシールド終了処理
void UpdatePlayerShield(void);								//プレイヤーシールド更新処理
void DrawPlayerShield(void);								//プレイヤーシールド描画処理
PLAYERSHIELD *GetPlayerShieldAdr(void);						//プレイヤーシールドアドレス取得関数
void SetPlayerShield(void);									//プレイヤーシールド接tpァmシイ
#endif
