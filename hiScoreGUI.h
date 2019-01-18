//=============================================================================
//
// スコアヘッダ [score.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _HISCORE_H_
#define _HISCORE_H_


// マクロ定義
#define HISCORE_TEXTURE_NAME	_T("data/TEXTURE/GUI/hiScoreGUI.png")				// スコアのテクスチャ

#define HISCOREBACK_TEXTURE_SIZE_X				(100)							//スコア背景テクスチャサイズX
#define HISCOREBACK_TEXTURE_SIZE_Y				(375.0f)						//スコア背景テクスチャサイズY

#define HISCORENUM_TEXTURE_SIZE_X				(12)							//数字テクスチャサイズX
#define HISCORENUM_TEXTURE_SIZE_Y				(16)							//数字テクスチャサイズY

#define HISCORE_INITPOS_X						(120)							//スコアのX座標
#define HISCORE_INITPOS_Y						(400)							//スコアのY座標

#define HISCORENUM_INITPOS_X					(70)							//スコア数字のX座標
#define HISCORENUM_INITPOS_Y					(-285)							//スコア数字のY座標
#define HISCORENUM_OFFSETPOS_X					(-15.0f)						//スコア数字のオフセット座標X
#define HISCORENUM_OFFSETPOS_Y					(63.0f)							//スコア数字のオフセット座標Y

#define HISCORE_DRAW_MAX						(10)								//表示するハイスコアの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHiScoreGUI(int num);								//スコア初期化処理
void UninitHiScoreGUI(void);									//スコア終了処理
void UpdateHiScoreGUI(void);									//スコア更新処理
void DrawHiscoreNum(void);									//スコア描画処理（数字用）
void DrawHiScoreGUIBack(void);									//スコア描画処理（背景用）

#endif
