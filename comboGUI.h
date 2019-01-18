//=============================================================================
//
// コンボGUIヘッダ [ComboGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _COMBOGUI_H_
#define _COMBOGUI_H_


// マクロ定義
#define COMBOGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/comboGUI.png")	// コンボGUIのテクスチャ

#define COMBOGUI_TEXTURE_SIZE_X					(100)				// テクスチャサイズX
#define COMBOGUI_TEXTURE_SIZE_Y					(75)				// テクスチャサイズY

#define COMBOGUI_INITPOS_X						(1160)				//コンボGUIの初期X座標
#define COMBOGUI_INITPOS_Y						(300)				//コンボGUIの初期Y座標

#define COMBOGUI_NUM_SIZE_X						(15)				//数字テクスチャのサイズX
#define COMBOGUI_NUM_SIZE_Y						(35)				//数字テクスチャのサイズY

#define COMBOGUI_NUM_INITPOS_X					(-20)				//数字テクスチャの初期座標X
#define COMBOGUI_NUM_INITPOS_Y					(25)				//数字テクスチャの初期座標Y
#define COMBOGUI_NUM_OFFSET						(-20.0f)			//数字テクスチャのオフセット値

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitComboGUI(int num);							//コンボGUI初期化処理
void UninitComboGUI(void);								//コンボGUI終了処理
void UpdateComboGUI(void);								//コンボGUI更新処理
void DrawComboGUI(void);								//コンボGUI描画処理（数字）
void DrawComboGUIBack(void);							//コンボGUI描画処理（背景）
#endif
