//=============================================================================
//
// トランジションヘッダ [Transition.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_


// マクロ定義
#define TRANSITION_TEXTURE_NAME	_T("data/TEXTURE/GUI/shutter.png")			//テクスチャへのパス

#define TRANSITION_TEXTURE_SIZE_X					(640)					// テクスチャサイズX
#define TRANSITION_TEXTURE_SIZE_Y					(240)					// テクスチャサイズY

#define TRANSITION_TEXTURE_DIVIDE_X					(1)						// アニメパターンのテクスチャ内分割数（X)
#define TRANSITION_TEXTURE_DIVIDE_Y					(2)						// アニメパターンのテクスチャ内分割数（Y)

#define TRANSITION_INITPOS_X						(SCREEN_CENTER_X)		//トランジションの初期X座標
#define TRANSITION_TOP_INITPOS_Y					(-TRANSITION_TEXTURE_SIZE_Y)				//上部トランジションの初期Y座標
#define TRANSITION_BOTTOM_INITPOS_Y					(SCREEN_HEIGHT+TRANSITION_TEXTURE_SIZE_Y)	//下部トランジションの初期Y座標


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	int						cntFrame;					//カウントフレーム
	int						state;						//遷移状態
	int						nextScene;					//次に遷移するシーン
}TRANSITION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTransition(int num);							//トランジション初期化処理
void UninitTransition(void);								//トランジション終了処理
void UpdateTransition(void);								//トランジション更新処理
void DrawTransition(void);									//トランジション描画処理
TRANSITION *GetTransitionAdr(void);							//トランジションアドレス取得関数
void SetTransition(int nextScene);							//遷移セット関数

#endif
