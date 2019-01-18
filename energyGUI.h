//=============================================================================
//
// エナジーGUIヘッダ [EnergyGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENERGYGUI_H_
#define _ENERGYGUI_H_


// マクロ定義
#define ENERGYGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/energyGauge.png")		// エナジーGUIのテクスチャ
#define ENERGYGUI_FRAMETEX_NAME	_T("data/TEXTURE/GUI/energyGaugeFrame.png") //エナジーGUIのフレームテクスチャ
#define ENERGYGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/EnergyGUI.png")		//エナジーGUIの背景テクスチャ

#define ENERGYGUI_TEXTURE_SIZE_X					(178)					// テクスチャサイズX
#define ENERGYGUI_TEXTURE_SIZE_Y					(133.0f)				// テクスチャサイズY

#define ENERGYGUI_BACKTEX_SIZE_X					(100)					//エナジーGUIの背景テクスチャサイズX
#define ENERGYGUI_BACKTEX_SIZE_Y					(75)					//エナジーGUIの背景テクスチャサイズY

#define ENERGYGUI_NUMTEX_SIZE_X						(12)					//エナジーGUIの数字テクスチャのサイズX
#define ENERGYGUI_NUMTEX_SIZE_Y						(12)					//エナジーGUIの数字テクスチャのサイズY

#define ENERGYGUI_INITPOS_X							(1160)					//エナジーGUIの初期X座標
#define ENERGYGUI_INITPOS_Y							(100)					//エナジーGUIの初期Y座標

#define ENERGYGUI_GAUGE_INITPOS_X					(-88)					//エナジーGUIのゲージ初期座標X
#define ENERGYGUI_GAUGE_INITPOS_Y					(-65)					//エナジーGUIのゲージ初期座標Y

#define ENERGYGUI_NUM_INITPOS_X						(-5)						//エナジーGUIの数字初期座標X
#define ENERGYGUI_NUM_INITPOS_Y						(47)					//エナジーGUIの数字初期座標Y
#define ENERGYGUI_NUM_OFFSET						(-15.0f)				//エナジーGUIの数字オフセット値
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnergyGUI(int num);							//エナジーGUI初期化処理
void UninitEnergyGUI(void);								//エナジーGUI終了処理
void UpdateEnergyGUI(void);								//エナジーGUI更新処理
void DrawEnergyGUI(void);								//エナジーGUI描画処理
void DrawEnergyGUIBack(void);							//エナジーGUI描画処理（背景用）
void DrawEnergyGUINum(void);							//エナジーGUI描画処理（数字用）
#endif
