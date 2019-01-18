//=============================================================================
//
// エフェクトファイアヘッダ [effectFire.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EFFECTFIRE_H_
#define _EFFECTFIRE_H_

#include "effect.h"

// マクロ定義
#define EFFECTFIRE_TEXTURE_NAME		_T("data/TEXTURE/effect/smallexplosion.png")		// テクスチャ名

#define EFFECTFIRE_TEXTURE_SIZE_X				(25.0f)
#define EFFECTFIRE_TEXTURE_SIZE_Y				(25.0f)

#define EFFECTFIRE_TEXTURE_DIVIDE_X				(4)								// アニメパターンのテクスチャ内分割数（X)
#define EFFECTFIRE_TEXTURE_DIVIDE_Y				(2)								// アニメパターンのテクスチャ内分割数（X)

#define EFFECTFIRE_ANIM_NUM			(EFFECTFIRE_TEXTURE_DIVIDE_X*EFFECTFIRE_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define EFFECTFIRE_ANIM_TIME					(3)								// アニメーションの切り替わるカウント

#define EFFECTFIRE_INITPOS_X					(-100)							//初期座標（X座標）
#define EFFECTFIRE_INITPOS_Y					(-100)							//初期座標（Y座標）

#define EFFECTFIRE_MAX							(32*32)							//各エフェクトファイアの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffectFire(int num);									//エフェクトファイア初期化処理
void UninitEffectFire(void);										//エフェクトファイア終了処理
void UpdateEffectFire(void);										//エフェクトファイア更新処理
void DrawEffectFire(void);											//エフェクトファイア描画処理
EFFECT *GetEffectFireAdr(int pno);									//エフェクトファイアのアドレス取得関数
EFFECT *SetEffectFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//エフェクトファイアのセット関数
EFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed);	//ボスファイアセット関数
#endif
