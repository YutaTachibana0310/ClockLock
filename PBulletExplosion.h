//=============================================================================
//
// プレイヤーバレットの爆発ヘッダ [PBulletExplosion.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PBULLETEXPLOSION_H_
#define _PBULLETEXPLOSION_H_


// マクロ定義
#define PBULLETEXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/bluefire.png")	// プレイヤーバレットの爆発のテクスチャ

#define PBULLETEXPLOSION_TEXTURE_SIZE_X					(20)					// テクスチャサイズX
#define PBULLETEXPLOSION_TEXTURE_SIZE_Y					(20)					// テクスチャサイズY

#define PBULLETEXPLOSION_TEXTURE_DIVIDE_X				(4)						// アニメパターンのテクスチャ内分割数（X)
#define PBULLETEXPLOSION_TEXTURE_DIVIDE_Y				(2)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PBULLETEXPLOSION_NUM			(PBULLETEXPLOSION_TEXTURE_DIVIDE_X*PBULLETEXPLOSION_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TIMEPBULLETEXPLOSION_ANIMATION					(4)						// アニメーションの切り替わるカウント

#define PBULLETEXPLOSION_MAX						(128)					//プレイヤーバレットの爆発の最大数（メインショット）

#define PBULLETEXPLOSION_INITPOS_X						(-100)					//プレイヤーバレットの爆発の初期X座標
#define PBULLETEXPLOSION_INITPOS_Y						(-100)					//プレイヤーバレットの爆発の初期Y座標

//プレイヤーバレットの爆発のバウンディングボックスの定義
#define PBULLETEXPLOSION_BB_TOP							(0.0f)
#define PBULLETEXPLOSION_BB_LEFT						(0.0f)
#define PBULLETEXPLOSION_BB_WIDTH						(10.0f)
#define PBULLETEXPLOSION_BB_HIGHT						(35.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;						// ポリゴンの角度

}PBULLETEXPLOSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPBulletExplosion(int num);
void UninitPBulletExplosion(void);
void UpdatePBulletExplosion(void);
void DrawPBulletExplosion(void);
PBULLETEXPLOSION *GetPBulletExplosionAdr(int pno);
void SetPBulletExplosion(D3DXVECTOR3 pos);

#endif
