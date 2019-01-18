//=============================================================================
//
// プレイヤーの爆発ヘッダ [PlayerExplosion.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYEREXPLOSION_H_
#define _PLAYEREXPLOSION_H_


// マクロ定義
#define PLAYEREXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/playerExplosion.png")	// プレイヤーの爆発のテクスチャ

#define PLAYEREXPLOSION_TEXTURE_SIZE_X					(80)					// テクスチャサイズX
#define PLAYEREXPLOSION_TEXTURE_SIZE_Y					(80)					// テクスチャサイズY

#define PLAYEREXPLOSION_TEXTURE_DIVIDE_X				(2)						// アニメパターンのテクスチャ内分割数（X)
#define PLAYEREXPLOSION_TEXTURE_DIVIDE_Y				(2)						// アニメパターンのテクスチャ内分割数（Y)
#define PLAYEREXPLOSION_ANIM_NUM			(PLAYEREXPLOSION_TEXTURE_DIVIDE_X*PLAYEREXPLOSION_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define PLAYEREXPLOSION_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define PLAYEREXPLOSION_MAX								(4)					//プレイヤーの爆発の最大数

#define PLAYEREXPLOSION_INITPOS_X						(-100)					//プレイヤーの爆発の初期X座標
#define PLAYEREXPLOSION_INITPOS_Y						(-100)					//プレイヤーの爆発の初期Y座標

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

}PLAYEREXPLOSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerExplosion(int num);							//プレイヤーの爆発初期化処理
void UninitPlayerExplosion(void);								//プレイヤーの爆発終了処理
void UpdatePlayerExplosion(void);								//プレイヤーの爆発更新処理
void DrawPlayerExplosion(void);									//プレイヤーの爆発描画処理
PLAYEREXPLOSION *GetPlayerExplosionAdr(int pno);				//プレイヤーの爆発アドレス取得関数
void SetPlayerExplosion(D3DXVECTOR3 pos);						//プレイヤー爆発のセット関数

#endif
