//=============================================================================
//
// スコアヘッダ [score.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//#include "main.h"

// マクロ定義
#define SCORE_TEXTURE_NAME	_T("data/TEXTURE/GUI/score.png")				// スコアのテクスチャ
#define SCORENUM_TEXTURE_NAME _T("data/TEXTURE/GUI/scoreNum.png")			//スコアの数字のテクスチャ

#define SCORE_TEXTURE_NUM					(2)								//スコアテクスチャの数

#define SCORENUM_TEXTURE_SIZE_X				(15)							//数字テクスチャサイズX
#define SCORENUM_TEXTURE_SIZE_Y				(20)							//数字テクスチャサイズY

#define SCOREBACK_TEXTURE_SIZE_X			(65)							//スコア背景テクスチャサイズX
#define SCOREBACK_TEXTURE_SIZE_Y			(20)							//スコア背景テクスチャサイズY

#define SCORENUM_TEXTURE_DIVIDE_X			(10)							// アニメパターンのテクスチャ内分割数（X)

#define SCORE_MAX							(8)								//スコアの最大桁数

#define SCORE_INITPOS_X						(310)							//スコアのX座標
#define SCORE_INITPOS_Y						(25)							//スコアのY座標

#define SCORENUM_INITPOS_X					(70)							//スコア数字のX座標
#define SCORENUM_INITPOS_Y					(0)							//スコア数字のY座標
#define SCORENUM_OFFSETPOS_X				(20.0f)						//スコア数字のオフセット座標X

#define SCOREBACK_INITPOS_X					(0.0f)							//スコア背景のX座標
#define SCOREBACK_INITPOS_Y					(30)							//スコア背景のY座標

#define HISCORE_MAX							(10)							//保存するハイスコアの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	int						score;										 //現在のハイスコア
	int						HiScore[HISCORE_MAX];						 //ハイスコア
	int						playerName[HISCORE_MAX][PLAYERNAME_MAX];	 //ハイスコアを達成したプレイヤー名
}SCOREDATA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int num);											//スコア初期化処理
void UninitScore(void);												//スコア終了処理
void UpdateScore(void);												//スコア更新処理
void DrawScore(void);												//スコア描画処理（数字用）
void DrawScoreBack(void);											//スコア描画処理（背景用）
SCOREDATA *GetScoreDataAdr(void);									//スコアデータアドレス取得関数	
void AddScore(int value);											//スコア加算処理
void CheckHiscoreUpdate(int *name);									//ハイスコア更新確認関数
void ShiftHiscoreName(int start);									//ハイスコアのネームをシフトする関数
bool isUpdateHighScore(void);										//ハイスコアが更新されているかチェック
#endif
