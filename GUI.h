//=============================================================================
//
// GUIヘッダ [GUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _GUI_H_
#define _PLAYERBYLLET_H_


// マクロ定義
#define GUI_TEXTURE_BACK _T("data/TEXTURE/GUI/GUIback.jpg")		// GUI背景テクスチャ
#define GUI_TEXTURE_SIZE_X			(960)						// テクスチャサイズ
#define GUI_TEXTURE_SIZE_Y			(960)						// テクスチャサイズ

#define GUI_QUAKE_LENGTHMAX			(10.0f)						//GUIが揺れるときの最大幅
#define BOSSGAUGE_OFFSET			(50.0f)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	D3DXVECTOR3				posR;						// ポリゴンの座標(右)
	D3DXVECTOR3				posL;						// ポリゴンの座標(左)
	int						cntFrame;					//カウントフレーム
	int						state;						//状態
}GUIback;

enum GUI_STATE
{
	GUI_DEFAULT,
	GUI_QUAKE,
	GUI_STATEMAX
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGUI(int num);								//GUI初期化処理
void UninitGUI(void);									//GUI終了処理
void UpdateGUI(void);									//GUI更新処理
void DrawGUI(void);										//GUI描画処理
GUIback *GetGUIbackAdr(void);							//GUIアドレス取得関数
void SetGUIShake(void);									//GUIを揺らす関数
void DrawParameter(void);
#endif
