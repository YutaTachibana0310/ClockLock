//=============================================================================
//
// スコアアイテムGUIヘッダ [ScoreItemGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SCOREITEMGUI_H_
#define _SCOREITEMGUI_H_


// マクロ定義
#define SCOREITEMGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/itemGUI.png") // スコアアイテムGUIのテクスチャ
#define SCOREITEMGUI_NUMTEX_NAME	_T("data/TEXTURE/GUI/scoreNum.png")  //スコアアイテムGUI背景のテクスチャ
#define SCOREITEMGUI_TEXTURE_NUM					(2)					//スコアアイテムGUIのテクスチャの数
		
#define SCOREITEMGUI_TEXTURE_SIZE_X					(100)				// テクスチャサイズX
#define SCOREITEMGUI_TEXTURE_SIZE_Y					(75)				// テクスチャサイズY

#define SCOREITEMGUI_NUMTEX_SIZE_X					(15)				//数字テクスチャのサイズ（X)
#define SCOREITEMGUI_NUMTEX_SIZE_Y					(20)				//数字テクスチャのサイズ（Y)

#define SCOREITEMGUI_TEXTURE_DIVIDE_X				(10)				// アニメパターンのテクスチャ内分割数（X)

#define SCOREITEMGUI_MAX							(5)					//スコアアイテムGUIの最大数

#define SCOREITEMGUI_INITPOS_X						(1160)				//スコアアイテムGUIの初期X座標
#define SCOREITEMGUI_INITPOS_Y						(500)				//スコアアイテムGUIの初期Y座標

#define SCOREITEMGUI_NUM_INITPOS_X					(70)				//数字テクスチャの初期座標（X)
#define SCOREITEMGUI_NUM_INITPOS_Y					(25)				 //数字テクスチャの初期座標（Y)
#define SCOREITEMGUI_NUM_OFFSETPOS					(-20.0f)			//数字テクスチャのオフセット座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScoreItemGUI(int num);							//スコアアイテムGUI初期化処理
void UninitScoreItemGUI(void);								//スコアアイテムGUI終了処理
void UpdateScoreItemGUI(void);								//スコアアイテムGUI更新処理
void DrawScoreItemGUI(void);								//スコアアイテムGUI描画処理（数字用）
void DrawScoreItemGUIBack(void);							//スコアアイテムGUI描画処理（背景用）
#endif
