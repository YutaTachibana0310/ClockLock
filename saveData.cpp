//=============================================================================
//
// 処理 [saveData.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "charChip.h"
#include "saveData.h"
#include "Score.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SAVEDATA_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
bool LoadSaveData(void);			//読込関数
bool CreateSaveData(void);			//作成関数
bool SetSaveData(void);				//保存関数

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
SAVEDATA saveData;				//プレイヤーバレット配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSaveData(int num)
{
	bool result = LoadSaveData();

	if (!result)
	{
		result = CreateSaveData();

		if (!result)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSaveData(void)
{
	SetSaveData();
}

/******************************************************************************
セーブデータ取得関数
******************************************************************************/
SAVEDATA *GetSaveDataAdr(void)
{
	return &saveData;
}

/******************************************************************************
セーブデータ読み込み関数
******************************************************************************/
bool LoadSaveData(void)
{
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "rb");

	if (fp == NULL)
	{
		return false;
	}

	SAVEDATA *ptr = GetSaveDataAdr();
	fread(ptr, sizeof(SAVEDATA), 1, fp);

	fclose(fp);
	return true;
}

/******************************************************************************
セーブデータ作成関数
******************************************************************************/
bool CreateSaveData(void)
{
	//セーブデータを初期化
	SAVEDATA *ptr = GetSaveDataAdr();
	for (int i = 0; i < SAVEDATA_HISCORE_MAX; i++)
	{
		ptr->name[i][0] = SAVEDATA_PLAYERNAME_DEFAULT1;
		ptr->name[i][1] = SAVEDATA_PLAYERNAME_DEFAULT2;
		ptr->name[i][2] = SAVEDATA_PLAYERNAME_DEFAULT3;
		ptr->name[i][3] = SAVEDATA_PLAYERNAME_DEFAULT4;
		ptr->name[i][4] = SAVEDATA_PLAYERNAME_DEFAULT5;

		ptr->highScore[i] = (SAVEDATA_HISCORE_MAX - i) * SAVEDATA_HISCORE_DEFAULT;
	}

	//ファイルへ保存
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "wb");

	if (fp == NULL)
	{
		return false;
	}

	fwrite(ptr, sizeof(SAVEDATA), 1, fp);

	fclose(fp);
	return true;
}

/******************************************************************************
セーブデータ保存関数
******************************************************************************/
bool SetSaveData(void)
{
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "wb");

	if (fp == NULL)
	{
		return false;
	}

	SAVEDATA *ptr = GetSaveDataAdr();
	SCOREDATA *scoreData = GetScoreDataAdr();

	for (int i = 0; i < SAVEDATA_HISCORE_MAX; i++)
	{
		ptr->highScore[i] = scoreData->HiScore[i];
		//strcpy(&ptr->hiScore[i].name[0], &scoreData->playerName[i][0]);
		//memcpy(&ptr->name[i][0], &scoreData->playerName[i][0], sizeof(ptr->name));
		ArrayCopy(&ptr->name[i][0], &scoreData->playerName[i][0], PLAYERNAME_MAX);
	}

	fwrite(ptr, sizeof(SAVEDATA), 1, fp);
	fclose(fp);
	return true;
}