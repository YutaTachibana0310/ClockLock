//=============================================================================
//
// 処理 [stageManager.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include <stdio.h>
#include "stageManager.h"
#include "enemyManager.h"
#include "player.h"
#include "bg.h"
#include "bgmPlayer.h"
#include "gameClear.h"
#include "sePlayer.h"
#include "warning.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define STAGEMANAGER_CLEAR_FRAMEMAX			(120)			//クリアの待機時間
#define STAGEMANAGER_PREBOSS_FRAMEMAX		(420)			//ボス前の待機時間

#define STAGEMANAGER_COMBOSCALE_MIN			(1.0f)			//コンボスケール最小値
#define STAGEMANAGER_COMBOSCALE_MAX			(1.5f)			//コンボスケール最大値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
bool LoadStageData(void);						//敵パターン読み込み関数
void PopEnemy(STAGEMANAGER *ptr);				//敵出現処理

//各状態のステージ処理
void ProcessDefaultStage(STAGEMANAGER *ptr);
void ProcessPreBossStage(STAGEMANAGER *ptr);
void ProcessBossBattleStage(STAGEMANAGER *ptr);
void ProcessClearStage(STAGEMANAGER *ptr);
/*****************************************************************************
構造体定義
*****************************************************************************/
enum STAGE1_ENEMYTYPE
{
	ENEMY_SMALLZAKO1,
	ENEMY_SMALLTANK,
	ENEMY_MIDDLETANK,
	ENEMY_SMALLPROPELLER,
	ENEMY_BIGENEMY1,
	ENEMY_SMALLZAKO2,
	ENEMT_STAGE1BOSS
};

enum STAGEMANAGER_SCALESTATE
{
	COMBO_DEFAULT,
	COMBO_ENLARGEMENT,
	COMBO_REDUCTION,
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static STAGEMANAGER stageManager;				//ステージマネージャー配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitStageManager(int num)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ptr->cntFrame = 0;
	ptr->lastPopFrame = 0;
	ptr->rank = 0;
	ptr->popNum = 0;

	//サンプラーステートパラメータを設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);		// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);		// テクスチャ縮小時の補間設定

	ptr->state = STAGE_DEFAULT;

	ptr->combo = 0;
	ptr->comboScale = 1.0f;
	ptr->scaleState = COMBO_DEFAULT;

	/*
	if (num == 1)
	{
		PlayBGM(BGM_STAGE1);
	}
	*/

	//敵パターンの読み込み
	bool result;

	if (num == 0)
	{
		result = LoadStageData();
		if (!result)
		{
			//ゲーム終了
			return S_FALSE;
		}
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitStageManager(void)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateStageManager(void)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();

	//スケールの設定
	if (ptr->scaleState == COMBO_ENLARGEMENT)
	{
		ptr->comboScale = Clampf(STAGEMANAGER_COMBOSCALE_MIN, STAGEMANAGER_COMBOSCALE_MAX, ptr->comboScale + 0.1f);
		if (ptr->comboScale == STAGEMANAGER_COMBOSCALE_MAX)
		{
			ptr->scaleState = COMBO_REDUCTION;
		}

	}
	else if (ptr->scaleState == COMBO_REDUCTION)
	{
		ptr->comboScale = Clampf(STAGEMANAGER_COMBOSCALE_MIN, STAGEMANAGER_COMBOSCALE_MAX, ptr->comboScale - 0.1f);
		if (ptr->comboScale == STAGEMANAGER_COMBOSCALE_MIN)
		{
			ptr->scaleState = COMBO_DEFAULT;
		}
	}

	switch (ptr->state)
	{
	case STAGE_DEFAULT:
		ProcessDefaultStage(ptr);
		break;

	case STAGE_PREBOSS:
		ProcessPreBossStage(ptr);
		break;

	case STAGE_BOSSBATTLE:
		ProcessBossBattleStage(ptr);
		break;

	case STAGE_CLEAR:
		ProcessClearStage(ptr);
		break;
	}

}

/******************************************************************************
DEFAULT処理
******************************************************************************/
void ProcessDefaultStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;
	int deltaFrame = ptr->cntFrame - ptr->lastPopFrame;

	//敵を出現させる
	if (ptr->popNum < ptr->dataMax)
	{
		if (deltaFrame >= ptr->data[ptr->popNum].interbal)
		{
			PopEnemy(ptr);
			ptr->lastPopFrame = ptr->cntFrame;
		}
	}
}

/******************************************************************************
PREBOSS処理
******************************************************************************/
void ProcessPreBossStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;

	//ボス出現タイミング？
	if (ptr->cntFrame == STAGEMANAGER_PREBOSS_FRAMEMAX)
	{
		SetStageState(STAGE_BOSSBATTLE);
		PlayBGM(BGM_BOSS);
		SetStage1Boss(ptr->data[ptr->popNum].initPos);
	}
}

/******************************************************************************
BOSSBATTLE処理
******************************************************************************/
void ProcessBossBattleStage(STAGEMANAGER *ptr)
{
	//ボス待機
}

/******************************************************************************
CLEAR処理
******************************************************************************/
void ProcessClearStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;
	//ちょっとしてからクリア処理
	if (ptr->cntFrame == STAGEMANAGER_CLEAR_FRAMEMAX)
	{
		SetGameClear();
		ptr->state = STAGE_STATEMAX;
	}
	
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawStageManager(void)
{

}

/******************************************************************************
//関数名	：STAGEMANAGER *GetStageManagerAdr(int pno)
//引数		：int pno：取得したいステージマネージャーのインデックス
//戻り値	：正常な引数：ステージマネージャーのアドレス	異常な引数：NULL
//説明		：ステージマネージャーのアドレス取得関数
******************************************************************************/
STAGEMANAGER *GetStageManagerAdr()
{
	return &stageManager;
}

/******************************************************************************
//関数名	：bool LoadStageData(void)
//引数		：void
//戻り値	：成功時：true		失敗時：false
//説明		：ステージの敵パターン読み込み関数
******************************************************************************/
bool LoadStageData(void)
{
	FILE *fp = NULL;
	STAGEMANAGER *ptr = GetStageManagerAdr();
	int loadResult;

	//ファイルの読み込み
	fp = fopen(STAGEMANAGER_DATA_PATH, "r");

	if (fp == NULL)
	{
		return false;
	}

	//敵パターンの数だけメモリを確保
	int dataNum = 0;
	loadResult = fscanf(fp, "%d", &dataNum);

	if (loadResult == EOF)
	{
		return false;
	}

	ptr->dataMax = Clamp(0, STAGEDATA_MAX, dataNum);

	//敵パターンを読み込み
	int tmpInterbal, tmpType;
	float tmpInitX, tmpInitY, tmpTargetX, tmpTargetY;

	for (int i = 0; i < ptr->dataMax; i++)
	{
		loadResult = fscanf(fp, "%d,%d,%f,%f,%f,%f", &tmpInterbal, &tmpType, &tmpInitX, &tmpInitY, &tmpTargetX, &tmpTargetY);
		ptr->data[i].interbal = tmpInterbal;
		ptr->data[i].enemyType = tmpType;
		ptr->data[i].initPos = D3DXVECTOR3(tmpInitX, tmpInitY, 0.0f);
		ptr->data[i].targetPos = D3DXVECTOR3(tmpTargetX, tmpTargetY, 0.0f);
	}

	fclose(fp);
	return true;
}

/******************************************************************************
//関数名	：void PopEnemy(STAGEMANAGER *ptr)
//引数		：STAGEMANAGER *ptr
//戻り値	：void
//説明		：敵パターン情報から敵を出現させる関数
******************************************************************************/
void PopEnemy(STAGEMANAGER *ptr)
{
	//各パラメータを読み込み
	int i = ptr->popNum;

	//パターンによって出現させる敵を分岐
	switch (ptr->data[i].enemyType)
	{
	case ENEMY_BIGENEMY1:
		SetBigEnemy(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_MIDDLETANK:
		CreateMiddleTankSetter(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLPROPELLER:
		SetSmallPropeller(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLTANK:
		CreateSmallTankSetter(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLZAKO1:
		SetSmallZako1(ptr->data[i].initPos);
		break;

	case ENEMY_SMALLZAKO2:
		SetSmallZako2(ptr->data[i].initPos);
		break;

	case ENEMT_STAGE1BOSS:
		SetStageState(STAGE_PREBOSS);
		FadeoutBGM(120);
		PlaySE(SE_WARN);

		//WARNING表示
		SetWarning();
		break;
	}

	ptr->popNum = Clamp(0, ptr->dataMax - 1, ptr->popNum + 1);
}

/******************************************************************************
//関数名	：void SetStageState(int state)
//引数		：int state
//戻り値	：void
//説明		：ステージの状態セット関数
******************************************************************************/
void SetStageState(int state)
{
	stageManager.state = state;
	stageManager.cntFrame = 0;
}

/******************************************************************************
//関数名	：void AddComboCount(int value)
//引数		：int value	：加算する値
//戻り値	：void
//説明		：コンボ数加算関数
******************************************************************************/
void AddComboCount(int value)
{
	stageManager.combo += value;

	if (stageManager.scaleState == COMBO_DEFAULT)
	{
		stageManager.scaleState = COMBO_ENLARGEMENT;
	}
}