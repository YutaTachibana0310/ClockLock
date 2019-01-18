//=============================================================================
//
// バレットファイアヘッダ [bulletFire.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BULLETFIRE_H_
#define _BULLETFIRE_H_

// マクロ定義
#define BULLETFIRE_TEXTURE_NAME		_T("data/TEXTURE/effect/bulletFireLumine.png")		// バレットファイアのテクスチャ

#define BULLETFIRE_TEXTURE_SIZE_X			(30 / 2)						// テクスチャサイズ（エネミー爆発用）
#define BULLETFIRE_TEXTURE_SIZE_Y			(50 / 2)						// テクスチャサイズ（エネミー爆発用）

#define BULLETFIRE_TEXTURE_DIVIDE_X			(2)								// アニメパターンのテクスチャ内分割数（X)
#define BULLETFIRE_TEXTURE_DIVIDE_Y			(1)								// アニメパターンのテクスチャ内分割数（X)

#define BULLETFIRE_ANIM_NUM			(BULLETFIRE_TEXTURE_DIVIDE_X*BULLETFIRE_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TIMEBULLETFIRE_ANIMATION			(2)								// アニメーションの切り替わるカウント

#define	BULLETFIRE_RADIUS_MIN				(50.0f)							// ポリゴンの半径最小値
#define	BULLETFIRE_RADIUS_MAX				(300.0f)						// ポリゴンの半径最大値
#define	BULLETFIRE_VALUE_ROTATION			(D3DX_PI * 0.01f)				// ポリゴンの回転量
#define	BULLETFIRE_VALUE_SCALE				(2.0f)							// ポリゴンのスケール変化量                                                                                          

#define BULLETFIRE_MAX						(4*PLAYER_MAX)					//バレットファイアの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量

	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							//ポリゴン半径
	float					angle;							//ポリゴン角度
}BULLETFIRE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletFire(int num);							//バレットファイア初期化処理
void UninitBulletFire(void);								//バレットファイア終了処理
void UpdateBulletFire(void);								//バレットファイア更新処理
void DrawBulletFire(void);									//バレットファイア描画処理
BULLETFIRE *GetBulletFireAdr(int pno);						//バレットファイアのアドレス取得関数
void SetBulletFire(BULLETFIRE * ptr);						//バレットファイアのセット関数

#endif
