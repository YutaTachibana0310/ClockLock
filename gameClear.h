//=============================================================================
//
// ゲームクリアヘッダ [GameClear.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _GAMECLEAR_H_
#define _GAMECLEAR_H_


// マクロ定義
#define GAMECLEAR_TEXTURE_NAME	_T("data/TEXTURE/bg/gameClear.png")	// ゲームクリアのテクスチャ
#define GAMECLEAR_CHARTEX_NAME	_T("data/TEXTURE/bg/clearText.png")	//テキストのテクスチャ

#define GAMECLEAR_TEXTURE_SIZE_X					(390)					// テクスチャサイズX
#define GAMECLEAR_TEXTURE_SIZE_Y					(480)					// テクスチャサイズY

#define GAMECLEAR_CHARTEX_SIZE_X					(380)					//テキストテクスチャのサイズX
#define GAMECLEAR_CHARTEX_SIZE_Y					(60)					//テキストテクスチャのサイズY
#define GAMECLEAR_CHATEX_DIVIDE_Y					(2)						//テクスチャ分割数

#define GAMECLEAR_INITPOS_X							(SCREEN_CENTER_X)		//ゲームクリアの初期X座標
#define GAMECLEAR_INITPOS_Y							(SCREEN_CENTER_Y)		//ゲームクリアの初期Y座標

#define GAMECLEAR_STAGETEX_INITPOS_X				(1280.0f)
#define GAMECLEAR_STAGETEX_INITPOS_Y				(100.0f)

#define GAMECLEAR_CLEARTEX_INITPOS_X				(0.0f)
#define GAMECLEAR_CLEARTEX_INITPOS_Y				(220.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用状態

	D3DXVECTOR3				pos;						// ポリゴンの座標

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度

	float					radiusChar;
	float					angleChar;

	int						cntFrame;					//カウントフレーム
	int						state;						//状態
}GAMECLEAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameClear(int num);							//ゲームクリア初期化処理
void UninitGameClear(void);								//ゲームクリア終了処理
void UpdateGameClear(void);								//ゲームクリア更新処理
void DrawGameClear(void);								//ゲームクリア描画処理
GAMECLEAR *GetGameClearAdr(void);						//ゲームクリアアドレス取得関数
void SetGameClear(void);								//ゲームクリアセット関数

#endif
