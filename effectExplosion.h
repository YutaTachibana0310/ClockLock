//=============================================================================
//
// エフェクトエクスプロージョンヘッダ [effectPiece.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EFFECTEXPLOSION_H_
#define _EFFECTEXPLOSION_H_

#include "effect.h"

// マクロ定義
#define EFFECTEXPLOSION_TEXTURE_NAME _T("data/TEXTURE/effect/explosion1.png")	// テクスチャ名

#define EFFECTEXPLOSION_TEXTURE_SIZE_X				(30.0f)
#define EFFECTEXPLOSION_TEXTURE_SIZE_Y				(30.0f)

#define EFFECTEXPLOSION_TEXTURE_DIVIDE_X			(8)								// アニメパターンのテクスチャ内分割数（X)
#define EFFECTEXPLOSION_TEXTURE_DIVIDE_Y			(2)								// アニメパターンのテクスチャ内分割数（X)

#define EFFECTEXPLOSION_ANIM_NUM			(EFFECTEXPLOSION_TEXTURE_DIVIDE_X*EFFECTEXPLOSION_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define EFFECTEXPLOSION_ANIM_TIME					(2)								// アニメーションの切り替わるカウント

#define EFFECTEXPLOSION_INITPOS_X					(-100)							//初期座標（X座標）
#define EFFECTEXPLOSION_INITPOS_Y					(-100)							//初期座標（Y座標）

#define EFFECTEXPLOSION_MAX							(32*32)							//各エフェクトエクスプロージョンの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffectExplosion(int num);									//エフェクトエクスプロージョン初期化処理
void UninitEffectExplosion(void);										//エフェクトエクスプロージョン終了処理
void UpdateEffectExplosion(void);										//エフェクトエクスプロージョン更新処理
void DrawEffectExplosion(void);											//エフェクトエクスプロージョン描画処理
EFFECT *GetEffectExplosionAdr(int pno);									//エフェクトエクスプロージョンのアドレス取得関数
EFFECT *SetEffectExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//エフェクトエクスプロージョンのセット関数

#endif
