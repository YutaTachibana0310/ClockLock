//=============================================================================
//
// エフェクトピースヘッダ [effectPiece.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EFFECTPIECE_H_
#define _EFFECTPIECE_H_

#include "effect.h"

// マクロ定義
#define EFFECTPIECE_TEXTURE_NAME _T("data/TEXTURE/effect/enemyPiece.png")	// テクスチャ名

#define EFFECTPIECE_TEXTURE_SIZE_X				(5.0f)
#define EFFECTPIECE_TEXTURE_SIZE_Y				(5.0f)

#define EFFECTPIECE_TEXTURE_DIVIDE_X			(8)								// アニメパターンのテクスチャ内分割数（X)
#define EFFECTPIECE_TEXTURE_DIVIDE_Y			(2)								// アニメパターンのテクスチャ内分割数（X)

#define EFFECTPIECE_ANIM_NUM					(16)	// アニメーションパターン数
#define EFFECTPIECE_ANIM_TIME					(1)								// アニメーションの切り替わるカウント

#define EFFECTPIECE_INITPOS_X					(-100)							//初期座標（X座標）
#define EFFECTPIECE_INITPOS_Y					(-100)							//初期座標（Y座標）

#define EFFECTPIECE_MAX							(32*32)							//各エフェクトピースの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffectPiece(int num);									//エフェクトピース初期化処理
void UninitEffectPiece(void);										//エフェクトピース終了処理
void UpdateEffectPiece(void);										//エフェクトピース更新処理
void DrawEffectPiece(void);											//エフェクトピース描画処理
EFFECT *GetEffectPieceAdr(int pno);									//エフェクトピースのアドレス取得関数
EFFECT *SetEffectPiece(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//エフェクトピースのセット関数

#endif
