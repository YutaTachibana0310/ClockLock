//=============================================================================
//
// グランドファイアヘッダ [GroundFire.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _GROUNDFIRE_H_
#define _GROUNDFIRE_H_


// マクロ定義
#define GROUNDFIRE_TEXTURE_NAME	_T("data/TEXTURE/effect/groundFire.png")	// グランドファイアのテクスチャ

#define GROUNDFIRE_TEXTURE_SIZE_X				(36)					// テクスチャサイズX
#define GROUNDFIRE_TEXTURE_SIZE_Y				(36)					// テクスチャサイズY

#define GROUNDFIRE_TEXTURE_DIVIDE_X				(4)						// アニメパターンのテクスチャ内分割数（X)
#define GROUNDFIRE_TEXTURE_DIVIDE_Y				(2)						// アニメパターンのテクスチャ内分割数（Y)
#define GROUNDFIRE_ANIM_NUM						(8)						// アニメーションパターン数
#define GROUNDFIRE_TIME_ANIMATION				(6)						// アニメーションの切り替わるカウント

#define GROUNDFIRE_MAX							(56)					//グランドファイアの最大数

#define GROUNDFIRE_INITPOS_X					(-100)					//グランドファイアの初期X座標
#define GROUNDFIRE_INITPOS_Y					(-100)					//グランドファイアの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						countAnim;					// アニメーションカウント
	int						patternAnim;				// アニメーションパターンナンバー

}GROUNDFIRE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGroundFire(int num);							//グランドファイア初期化処理
void UninitGroundFire(void);								//グランドファイア終了処理
void UpdateGroundFire(void);								//グランドファイア更新処理
void DrawGroundFire(void);									//グランドファイア描画処理
GROUNDFIRE *GetGroundFireAdr(int pno);						//グランドファイアアドレス取得関数
void SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//グランドファイアセット関数
#endif