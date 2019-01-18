//=============================================================================
//
// 残機GUIヘッダ [ZankiGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ZANKIGUI_H_
#define _ZANKIGUI_H_


// マクロ定義
#define ZANKIGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/zanki.png")		//残機GUIのテクスチャ（アイコン）
#define ZANKIGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/zankiBack.png")	//残機GUIのテクスチャ（背景用）

#define ZANKIGUI_TEXTURE_NUM					(2)						//テクスチャの数

#define ZANKIGUI_TEXTURE_SIZE_X					(30)					//テクスチャサイズX（アイコン用）
#define ZANKIGUI_TEXTURE_SIZE_Y					(30)					//テクスチャサイズY（アイコン用）

#define ZANKIGUI_BACKTEX_SIZE_X					(100)					// テクスチャサイズX(背景用)
#define ZANKIGUI_BACKTEX_SIZE_Y					(75)					// テクスチャサイズY(背景用)

#define ZANKIGUI_INITPOS_X						(-30)					//残機GUIの初期X座標（アイコン用）
#define ZANKIGUI_INITPOS_Y						(0)					//残機GUIの初期Y座標（アイコン用）

#define ZANKIGUI_BACK_INITPOS_X					(960)					//残機GUIの初期X座標（背景用）
#define ZANKIGUI_BACK_INITPOS_Y					(30)					//残機GUIの初期Y座標（背景用）

#define ZANKIGUI_CHAR_SIZE_X					(10)					//文字テクスチャのサイズX
#define ZANKIGUI_CHAR_SIZE_Y					(10)					//文字テクスチャのサイズY
#define ZANKIGUI_CHAR_INITPOS_X					(10.0f)					//文字テクスチャの初期座標X
#define ZANKIGUI_CHAR_INITPOS_Y					(0.0f)					//文字テクスチャの初期座標Y

#define ZANKIGUI_OFFSETPOS						(30.0f)					//残機アイコンのオフセット値

#define ZANKIGUI_NUM_INITPOS_X					(35.0f)					//残機数の初期座標X
#define ZANKIGUI_NUM_INITPOS_Y					(0.0f)					//残機数の初期座標Y
#define ZANKIGUI_NUM_OFFSETPOS					(20.0f)					//残機数のオフセット
#define ZANKIGUI_NUM_SIZE_X						(15)
#define ZANKIGUI_NUM_SIZE_Y						(25)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitZankiGUI(int num);							//残機GUI初期化処理
void UninitZankiGUI(void);								//残機GUI終了処理
void UpdateZankiGUI(void);								//残機GUI更新処理
void DrawZankiGUI(void);								//残機GUI描画処理(アイコン用)
void DrawZankiGUIBack(void);							//残機GUI描画処理（背景用）

#endif
