//=============================================================================
//
// スコア処理 [score.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "score.h"
#include "saveData.h"
#include "stageManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexScore(void);							//頂点作成関数
void SetTextureScore(void);								//テクスチャ座標の計算処理
void SetTextureScoreNum(int cntPattern);				//テクスチャ座標の計算処理
void SetVertexScore(void);								//頂点の計算処理
void SetVertexScoreNum(float offset);					//頂点の計算処理
void SetVertexScoreBack(void);							//頂点の計算処理

enum SCORETEXTURE_INDEX
{
	TEXTURE_SCOREBACK,
	TEXTURE_SCORENUM,
};

enum {
	DEFAULT,
	ENLARGEMENT,
	REDUCTION
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SCORE_TEXTURE_NUM] = { NULL, NULL };					// スコアテクスチャへのポインタ

static VERTEX_2D				vertexWk[NUM_VERTEX];						//頂点情報格納ワーク

static float					radius, radiusNum, radiusBack;				//ポリゴンの頂点への長さ
static float					angle, angleNum, angleBack;					//ポリゴンの頂点への角度

static float					scale;										//ポリゴンのスケール
static int						state;										//描画ステート
static int						digitMax;									//表示する桁の最大数
static D3DXVECTOR3				basePos;									//基準座標

SCOREDATA						scoreData;									//スコアデータ構造体

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScore(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREDATA *ptr = GetScoreDataAdr();
	SAVEDATA *saveData = GetSaveDataAdr();

	ptr->score = 0;

	//基準座標の設定
	basePos = D3DXVECTOR3(SCORE_INITPOS_X, SCORE_INITPOS_Y, 0.0f);

	//各パラメータの初期化
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(SCORENUM_TEXTURE_SIZE_X, SCORENUM_TEXTURE_SIZE_Y));
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(SCOREBACK_TEXTURE_SIZE_X, SCOREBACK_TEXTURE_SIZE_Y));

	angleNum = atan2f(SCORENUM_TEXTURE_SIZE_Y / 2, SCORENUM_TEXTURE_SIZE_X / 2);
	angleBack = atan2f(SCOREBACK_TEXTURE_SIZE_Y / 2, SCOREBACK_TEXTURE_SIZE_X / 2);

	scale = 1.0f;
	state = DEFAULT;
	digitMax = 1;

	// 頂点情報の作成
	MakeVertexScore();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[TEXTURE_SCORENUM] = CreateTextureFromFile((LPSTR)SCORENUM_TEXTURE_NAME, pDevice);
		texture[TEXTURE_SCOREBACK] = CreateTextureFromFile((LPSTR)SCORE_TEXTURE_NAME, pDevice);

		//セーブデータからハイスコア情報の読込
		for (int i = 0; i < HISCORE_MAX; i++)
		{
			ptr->HiScore[i] = saveData->highScore[i];
			//strcpy(&ptr->playerName[i][0], &saveData->hiScore[i].name[0]);
			//memcpy(&ptr->playerName[i][0], &saveData->name[i][0], sizeof(ptr->playerName[i]));
			ArrayCopy(&ptr->playerName[i][0], &saveData->name[i][0], PLAYERNAME_MAX);
		}
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScore(void)
{
	int i;
	for (i = 0; i < SCORE_TEXTURE_NUM; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateScore(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(SCORE_INITPOS_X, SCORE_INITPOS_Y, 0.0f);
	
	if (GetStageManagerAdr()->state == STAGE_BOSSBATTLE)
	{
		basePos.y += BOSSGAUGE_OFFSET;
	}

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}

	//拡大処理
	if (state == ENLARGEMENT)
	{
		scale += 0.05f;

		if (scale >= 1.5f)
		{
			state = REDUCTION;
			return;
		}
	}
	//縮小処理
	else if (state == REDUCTION)
	{
		scale -= 0.05f;
		if (scale <= 1.0f)
		{
			state = DEFAULT;
			return;
		}
	}
}

/******************************************************************************
描画処理(数字用）
******************************************************************************/
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 offsetPos = D3DXVECTOR3(SCORENUM_INITPOS_X, SCORENUM_INITPOS_Y, 0.0f);
	SCOREDATA *ptr = GetScoreDataAdr();
	int i, tmpScore, data;
	float offset = 0.0f;

	DrawScoreBack();

	//スコア数字の描画
	pDevice->SetTexture(0, texture[TEXTURE_SCORENUM]);
	tmpScore = ptr->score;
	for (i = 0; i < digitMax; i++, tmpScore /= 10)
	{
		data = tmpScore % 10;
		//offsetPos.x = (float)SCORENUM_INITPOS_X - i * SCORENUM_OFFSETPOS_X;
		offset = i * SCORENUM_OFFSETPOS_X;

		//頂点座標を設定
		SetVertexScoreNum(offset);

		// テクスチャ座標を設定
		SetTextureScoreNum(data);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
描画処理(背景用）
******************************************************************************/
void DrawScoreBack(void)
{
	PDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//スコア背景の描画
	pDevice->SetTexture(0, texture[TEXTURE_SCOREBACK]);
	SetTextureScore();
	SetVertexScoreBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureScore(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

void SetTextureScoreNum(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % SCORENUM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCORENUM_TEXTURE_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexScore(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = basePos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = basePos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = basePos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = basePos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = basePos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = basePos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = basePos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = basePos.y + sinf(angle) * radius;
}

/******************************************************************************
頂点座標の設定(数字用)
******************************************************************************/
void SetVertexScoreNum(float offset)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(SCORENUM_INITPOS_X, SCORENUM_INITPOS_Y, 0.0f);
	pos.x += SCORENUM_OFFSETPOS_X * (digitMax - 1);
	pos.x -= offset;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
}

/******************************************************************************
頂点座標の設定(背景用)
******************************************************************************/
void SetVertexScoreBack(void)
{
	//頂点座標の設定
	vertexWk[0].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[0].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[1].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[1].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[2].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[2].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
	vertexWk[3].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[3].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
}

/******************************************************************************
//関数名	：SCORE *GetScoreAdr(void)
//引数		：void
//戻り値	：スコアデータのアドレス
//説明		：スコアデータアドレス取得関数
******************************************************************************/
SCOREDATA *GetScoreDataAdr(void)
{
	return &scoreData;
}

/******************************************************************************
//関数名	：SCORE *GetScoreAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（メインショット）
******************************************************************************/
void AddScore(int value)
{
	SCOREDATA *ptr = GetScoreDataAdr();

	ptr->score += value;

	if (ptr->score >= pow(10, SCORE_MAX))
	{
		ptr->score = (int)pow(10, SCORE_MAX) - 1;
	}

	if (state == DEFAULT)
	{
		state = ENLARGEMENT;
	}

	//表示桁の処理
	int digit = (int)log10((double)ptr->score) + 1;
	digitMax = digit;
}

/******************************************************************************
//関数名	：void CheckHiscoreUpdate(void)
//引数		：void
//戻り値	：void
//説明		：ハイスコアの更新があるかどうかのチェック
******************************************************************************/
void CheckHiscoreUpdate(int *name)
{
	SCOREDATA *ptr = GetScoreDataAdr();
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		if (ptr->score != Max(ptr->score, ptr->HiScore[i]))
		{
			continue;
		}

		ArrayShiftDown(&ptr->HiScore[0], i, HISCORE_MAX);
		ptr->HiScore[i] = ptr->score;

		ShiftHiscoreName(i);
		ArrayCopy(&ptr->playerName[i][0], &name[0], PLAYERNAME_MAX);
		break;

	}
}

/******************************************************************************
//関数名	：bool isUpdateHighScore(void)
//引数		：void
//戻り値	：void
//説明		：スコアがハイスコアを更新しているかチェック
******************************************************************************/
bool isUpdateHighScore(void)
{
	SCOREDATA *ptr = &scoreData;

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		if (ptr->score == Max(ptr->score, ptr->HiScore[i]))
		{
			return true;
		}
	}

	return false;
}

/******************************************************************************
//関数名	：void ShiftRankingName(int start)
//引数		：int start		：シフトの開始位置
//戻り値	：void
//説明		：ハイスコアのネームを開始位置から下にシフトする関数
******************************************************************************/
void ShiftHiscoreName(int start)
{
	SCOREDATA *ptr = GetScoreDataAdr();

	for (int i = HISCORE_MAX - 1; i > start; i--)
	{
		for (int j = 0; j < PLAYERNAME_MAX; j++)
		{
			ptr->playerName[i][j] = ptr->playerName[i - 1][j];
		}
	}
}