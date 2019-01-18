//=============================================================================
//
// ボムGUIヘッダ [BomGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BOMGUI_H_
#define _BOMGUI_H_


// マクロ定義
#define BOMGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/bomIcon.png")			//ボムGUIのテクスチャ（アイコン）
#define BOMGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/bomGUI.png")			//ボムGUIのテクスチャ（背景用）

#define BOMGUI_TEXTURE_NUM						(2)						//テクスチャの数

#define BOMGUI_TEXTURE_SIZE_X					(25)					//テクスチャサイズX（アイコン用）
#define BOMGUI_TEXTURE_SIZE_Y					(25)					//テクスチャサイズY（アイコン用）

#define BOMGUI_BACKTEX_SIZE_X					(100)					// テクスチャサイズX(背景用)
#define BOMGUI_BACKTEX_SIZE_Y					(75)					// テクスチャサイズY(背景用)

#define BOMGUI_INITPOS_X						(0)					//ボムGUIの初期X座標（アイコン用）
#define BOMGUI_INITPOS_Y						(20)					//ボムGUIの初期Y座標（アイコン用）

#define BOMGUI_BACK_INITPOS_X					(1000)					//ボムGUIの初期X座標（背景用）
#define BOMGUI_BACK_INITPOS_Y					(60)					//ボムGUIの初期Y座標（背景用）

#define BOMGUI_OFFSETPOS						(-30.0f)					//ボムアイコンのオフセット値

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBomGUI(int num);							//ボムGUI初期化処理
void UninitBomGUI(void);								//ボムGUI終了処理
void UpdateBomGUI(void);								//ボムGUI更新処理
void DrawBomGUI(void);								//ボムGUI描画処理(アイコン用)
void DrawBomGUIBack(void);							//ボムGUI描画処理（背景用）

#endif
