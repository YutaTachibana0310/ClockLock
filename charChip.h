//=============================================================================
//
// 文字チップヘッダ [CharChip.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _CHARCHIP_H_
#define _CHARCHIP_H_

// マクロ定義
#define CHARCHIP_TEXTURE_NAME	_T("data/TEXTURE/GUI/charchip.png")	// 文字チップのテクスチャ

#define CHARCHIP_TEXTURE_DIVIDE_X				(15)				// テクスチャ内分割数（X)
#define CHARCHIP_TEXTURE_DIVIDE_Y				(2)					// テクスチャ内分割数（Y)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
enum DEFINE_CHARCHIP
{
	CHARCHIP_A,
	CHARCHIP_B,
	CHARCHIP_C,
	CHARCHIP_D,
	CHARCHIP_E,
	CHARCHIP_F,
	CHARCHIP_G,
	CHARCHIP_H,
	CHARCHIP_I,
	CHARCHIP_J,
	CHARCHIP_K,
	CHARCHIP_L,
	CHARCHIP_M,
	CHARCHIP_N,
	CHARCHIP_O,
	CHARCHIP_P,
	CHARCHIP_Q,
	CHARCHIP_R,
	CHARCHIP_S,
	CHARCHIP_T,
	CHARCHIP_U,
	CHARCHIP_V,
	CHARCHIP_W,
	CHARCHIP_X,
	CHARCHIP_Y,
	CHARCHIP_Z,
	CHARCHIP_ASTERISK,
	CHARCHIP_EXCLAMATION,
	CHARCHIP_QUESTION,
	CHARCHIP_END,
	CHARCHIP_MAX,
	CHARCHIP_NONE
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCharChip(int num);							//文字チップ初期化処理
void UninitCharChip(void);								//文字チップ終了処理
void DrawCharChip(VERTEX_2D *vertexWk, int charchipID);		//文字チップ描画処理

#endif
