//=============================================================================
//
// セーブデータヘッダ [SaveData.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SAVEDATA_H_
#define _SAVEDATA_H_

#include "main.h"

// マクロ定義
#define SAVEDATA_FILE_PATH ("data/save.dat")						//セーブデータのファイルへのパス

#define SAVEDATA_HISCORE_MAX				(10)					//保存されるハイスコアの最大数

#define SAVEDATA_PLAYERNAME_DEFAULT1			(CHARCHIP_S)					//デフォルトプレイヤー名
#define SAVEDATA_PLAYERNAME_DEFAULT2			(CHARCHIP_F)					//デフォルトプレイヤー名
#define SAVEDATA_PLAYERNAME_DEFAULT3			(CHARCHIP_ASTERISK)				//デフォルトプレイヤー名
#define SAVEDATA_PLAYERNAME_DEFAULT4			(CHARCHIP_ASTERISK)					//デフォルトプレイヤー名
#define SAVEDATA_PLAYERNAME_DEFAULT5			(CHARCHIP_ASTERISK)					//デフォルトプレイヤー名

#define SAVEDATA_HISCORE_DEFAULT			(10000)					//デフォルトハイスコア

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	int highScore[SAVEDATA_HISCORE_MAX];
	int name[SAVEDATA_HISCORE_MAX][PLAYERNAME_MAX];

}SAVEDATA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSaveData(int num);							//セーブデータ初期化処理
void UninitSaveData(void);								//セーブデータ終了処理
SAVEDATA *GetSaveDataAdr(void);						//セーブデータアドレス取得関数

#endif
