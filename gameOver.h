//=============================================================================
//
// ゲームオーバーヘッダ [GameOver.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_


// マクロ定義
#define GAMEOVER_TEXTURE_NAME	_T("data/TEXTURE/bg/gameover.png")	// ゲームオーバーのテクスチャ

#define GAMEOVER_TEXTURE_SIZE_X					(390)					// テクスチャサイズX
#define GAMEOVER_TEXTURE_SIZE_Y					(480)					// テクスチャサイズY

#define GAMEOVER_INITPOS_X						(SCREEN_CENTER_X)					//ゲームオーバーの初期X座標
#define GAMEOVER_INITPOS_Y						(SCREEN_CENTER_Y)					//ゲームオーバーの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用状態

	D3DXVECTOR3				pos;						// ポリゴンの座標

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度
	int						cntFrame;					//カウントフレーム
	int						state;						//状態
}GAMEOVER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameOver(int num);							//ゲームオーバー初期化処理
void UninitGameOver(void);								//ゲームオーバー終了処理
void UpdateGameOver(void);								//ゲームオーバー更新処理
void DrawGameOver(void);								//ゲームオーバー描画処理
GAMEOVER *GetGameOverAdr(void);							//ゲームオーバーアドレス取得関数
void SetGameOver(void);									//ゲームオーバーセット関数

#endif
