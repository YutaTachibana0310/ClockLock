//=============================================================================
//
// ポーズメニューヘッダ [PauseMenu.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_


// マクロ定義
#define PAUSEMENU_TEXTURE_NAME	_T("data/TEXTURE/GUI/pausemenu.png")		// ポーズメニューのテクスチャ
#define PAUSEMENU_CURSORTEX_NAME _T("data/TEXTURE/GUI/pauseLumine.png")		//ポーズメニューのカーソルテクスチャ

#define PAUSEMENU_TEXTURE_SIZE_X					(150)					// テクスチャサイズX
#define PAUSEMENU_TEXTURE_SIZE_Y					(150)					// テクスチャサイズY

#define PAUSEMENU_CURSORTEX_DIVIDE_X				(3)						// アニメパターンのテクスチャ内分割数（X)

#define PAUSEMENU_MAX								(1)						//ポーズメニューの最大数

#define PAUSEMENU_INITPOS_X							(SCREEN_CENTER_X)		//ポーズメニューの初期X座標
#define PAUSEMENU_INITPOS_Y							(SCREEN_CENTER_Y)		//ポーズメニューの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	int						select;						//選択されているメニュー
	float					scale;						//スケール
	int						state;						//状態
	int						cntFrame;					//カウントフレーム
}PAUSEMENU;

//ポーズメニューの状態列挙型
enum PAUSEMENU_STATE
{
	PAUSEMENU_OPEN,
	PAUSEMENU_SELECT,
	PAUSEMENU_CLOSE,
	PAUSEMENU_NONE,
	PAUSEMENU_STATEMAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPauseMenu(int num);							//ポーズメニュー初期化処理
void UninitPauseMenu(void);								//ポーズメニュー終了処理
void UpdatePauseMenu(void);								//ポーズメニュー更新処理
void DrawPauseMenu(void);								//ポーズメニュー描画処理
PAUSEMENU *GetPauseMenuAdr(int pno);					//ポーズメニューアドレス取得関数
void SetPauseMenuState(PAUSEMENU_STATE state);			//ポーズメニューの状態セット関数

#endif
