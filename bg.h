//=============================================================================
//
// 背景ヘッダー [bg.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "enemyBullet.h"

// マクロ定義
#define BG_TEXTURENAME_STAGE1_0	_T("data/TEXTURE/bg/bg_stage1_0.png")	// テクスチャ名
#define BG_TEXTURENAME_STAGE1_1	_T("data/TEXTURE/bg/bg_stage1_1.png")	// テクスチャ名
#define BG_TEXTURENAME_STAGE1_2	_T("data/TEXTURE/bg/bg_stage1_2.png")	// テクスチャ名

#define BG_TEXTURENAME_EFFECT1 _T("data/TEXTURE/bg/effect1.png")	// テクスチャ名

#define BG_TEXTURESIZE_X		(780)								// テクスチャサイズ 4:5
#define BG_TEXTURESIZE_Y		(960)								// テクスチャサイズ 

#define BG_POS_X			((SCREEN_WIDTH-BG_TEXTURESIZE_X)/2)		//背景テクスチャのX座標

#define BG_LEFT					(BG_POS_X)							//ゲーム画面左端
#define BG_RIGHT				(BG_POS_X+BG_TEXTURESIZE_X)			//ゲーム画面右端

#define BG_MAX					(3)									//背景の最大数

#define BG_STAGE1_SPEED1		(-0.0005f)							//背景のスクロールスピード（遠景）
#define BG_STAGE1_SPEED2		(-0.0007f)							//背景のスクロールスピード（中景）
#define BG_STAGE1_SPEED3		(-0.0012f)							//背景のスクロールスピード（近景）

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	tex;							//テクスチャへのポインタ
	float				offsetSpeedX;					//背景のスクロール速度	
	float				offsetSpeedY;						
	float				offsetX;						//背景のオフセット値
	float				offsetY;
}BG;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int num);					//背景の初期化処理
void UninitBG(void);						//背景の終了処理
void UpdateBG(void);						//背景の更新処理
void DrawBG(void);							//背景の描画処理
void SetTimeEffect(SPEEDTYPE type);			//時間操作エフェクト用の背景セット関数
void DrawTimeEffect(void);					//時間操作エフェクトの描画処理
#endif
