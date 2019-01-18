//=============================================================================
//
// ネームエントリー処理 [nameEntry.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "charChip.h"
#include "nameEntry.h"
#include "input.h"
#include "saveData.h"
#include "Score.h"
#include "transition.h"
#include "sePlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
//頂点作成関数
HRESULT MakeVertexNameEntry(void);

// テクスチャ座標の計算処理
void SetTextureNameEntry(void);					
void SetTextureNameEntryCursor(void);
void SetTextureNameEntryGuide(void);

// 頂点の計算処理
void SetVertexNameEntryChar(int charID);		
void SetVertexNameEntryCursor(int charID);
void SetVertexNameEntryName(int wordCnt);
void SetVertexNameEntryBG(void);
void SetVertexNameEntryScore(float offsetX, float offsetY);
void SetVertexNameEntryScoreName(float offsetX, float offsetY);
void SetVertexNameEntryCurrentScore(float offset);
void SetVertexNameEntryGuide(void);

//ネームエントリー描画処理（文字用）
void DrawNameEntryChar(void);					
void DrawNameEntryName(void);					
void DrawNameEntryCursor(void);
void DrawNameEntryBG(void);
void DrawNameEntryHighScore(void);
void DrawNameEntryScoreName(void);
void DrawNameEntryCurrentScore(void);
void DrawNameEntryGuide(void);

bool isDrawSelectedChar(int i);					//選択中の文字の描画判定
void SelectPlayerName();						//プレイヤー名入力処理
void CheckUpdateRanking(void);					//ハイスコア配列の更新

/*****************************************************************************
構造体定義
*****************************************************************************/
enum NAMEENTRY_TEXTURE
{
	NAMEENTRY_BACK,
	NAMEENTRY_CURSOR,
	NAMEENTRY_GUIDE,
	NAMEENTRY_TEXMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[NAMEENTRY_TEXMAX];		// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];						//頂点情報格納ワーク
static int buffHighScore[HISCORE_MAX];						//描画用ハイスコア配列	
static int buffHighScoreName[HISCORE_MAX][PLAYERNAME_MAX];	//描画用プレイヤー名配列

NAMEENTRY nameEntry;										//ネームエントリー構造体

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitNameEntry(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	NAMEENTRY *ptr = &nameEntry;

	//各ポリゴン用パラメータを初期化
	ptr->angleChar = atan2f(NAMEENTRY_CHARTEX_SIZE_Y, NAMEENTRY_CHARTEX_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_CHARTEX_SIZE_X, NAMEENTRY_CHARTEX_SIZE_Y));

	ptr->angleCursor = atan2f(NAMEENTRY_CURSORTEX_SIZE_Y, NAMEENTRY_CURSORTEX_SIZE_X);
	ptr->radiusCursor = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_CURSORTEX_SIZE_X, NAMEENTRY_CURSORTEX_SIZE_Y));

	ptr->angleName = atan2f(NAMEENTRY_NAMETEX_SIZE_Y, NAMEENTRY_NAMETEX_SIZE_X);
	ptr->radiusName = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_NAMETEX_SIZE_X, NAMEENTRY_NAMETEX_SIZE_Y));

	ptr->angleScore = atan2f(NAMEENTRY_SCORETEX_SIZE_Y, NAMEENTRY_SCORETEX_SIZE_X);
	ptr->radiusScore = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_SCORETEX_SIZE_X, NAMEENTRY_SCORETEX_SIZE_Y));

	ptr->cntFrame = 0;
	ptr->wordCnt = 0;
	ptr->selectedChar = 0;

	ptr->state = NAMEENTRY_INPUT;

	//入力用プレイヤー名を初期化
	for (int i = 0; i < PLAYERNAME_MAX; i++)
	{
		ptr->nameBuff[i] = CHARCHIP_NONE;
	}

	// 頂点情報の作成
	MakeVertexNameEntry();

	//サンプラーステートパラメータを設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定

	//現在のハイスコアを読込
	//SAVEDATA *save = GetSaveDataAdr();
	SCOREDATA *data = GetScoreDataAdr();
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		//buffHighScore[i] = save->highScore[i];
		buffHighScore[i] = data->HiScore[i];
		ArrayCopy(&buffHighScoreName[i][0], &data->playerName[i][0], PLAYERNAME_MAX);
	}

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[NAMEENTRY_BACK] = CreateTextureFromFile((LPSTR)NAMEENTRY_TEXTURE_NAME, pDevice);
		texture[NAMEENTRY_CURSOR] = CreateTextureFromFile((LPSTR)NAMEENTRY_CURSORTEX_NAME, pDevice);
		texture[NAMEENTRY_GUIDE] = CreateTextureFromFile((LPSTR)NAMEENTRY_GUIDETEX_NAME, pDevice);
	}

	//ハイスコアの更新がなければ名前入力を行わない
	ptr->state = (isUpdateHighScore()) ? NAMEENTRY_INPUT : NAMEENTRY_WAIT;


	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitNameEntry(void)
{
	for (int i = 0; i < NAMEENTRY_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateNameEntry(void)
{
	NAMEENTRY *ptr = &nameEntry;

	ptr->cntFrame = WrapAround(0, NAMEENTRY_CURSORTEX_ANIMAX, ptr->cntFrame + 1);

	//名前入力
	if (ptr->state == NAMEENTRY_INPUT)
	{
		SelectPlayerName();
		return;
	}

	//ボタンが押されたらタイトルへ遷移
	bool input = GetMyButton0Trigger(0) | GetMyButton1Trigger(0) | GetMyButton2Trigger(0);
	if(ptr->state == NAMEENTRY_WAIT && input)
	{
		SetTransition(SCENE_TITLE);
		ptr->state = NAMEENTRY_STATEMAX;
	}
}

/******************************************************************************
プレイヤー名入力処理
******************************************************************************/
void SelectPlayerName()
{
	NAMEENTRY *ptr = &nameEntry;

	//選択されている文字の更新
	int x = ptr->selectedChar % CHARCHIP_TEXTURE_DIVIDE_X;
	int y = ptr->selectedChar / CHARCHIP_TEXTURE_DIVIDE_X;

	x = WrapAround(0, CHARCHIP_TEXTURE_DIVIDE_X, x + GetMyAxisXRepeat(0));
	y = WrapAround(0, CHARCHIP_TEXTURE_DIVIDE_Y, y + GetMyAxisYRepeat(0));
	ptr->selectedChar = x + y * CHARCHIP_TEXTURE_DIVIDE_X;

	//カーソル移動時のSE再生
	if (GetMyAxisXRepeat(0) != 0 || GetMyAxisYRepeat(0) != 0)
	{
		PlaySE(SE_CURSOR);
	}

	//入力
	if (GetMyButton0Trigger(0))
	{
		PlaySE(SE_DECIDE);
		if (ptr->selectedChar == CHARCHIP_END)
		{
			if (ptr->wordCnt > 0)
			{
				//ネームエントリー修了
				ptr->state = NAMEENTRY_WAIT;
				CheckUpdateRanking();
			}
			return;
		}

		//文字をプレイヤー名に入れ、次の文字へ
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX - 1, ptr->wordCnt);
		ptr->nameBuff[ptr->wordCnt] = ptr->selectedChar;
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX, ptr->wordCnt + 1);

		//文字数が最大ならENDへ文字送り
		if (ptr->wordCnt == PLAYERNAME_MAX)
		{
			ptr->selectedChar = CHARCHIP_END;
		}
		return;
	}

	//一文字戻る
	if (GetMyButton2Trigger(0))
	{
		ptr->nameBuff[ptr->wordCnt] = CHARCHIP_NONE;
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX - 1, ptr->wordCnt - 1);
		return;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawNameEntry(void)
{
	DrawNameEntryBG();
	DrawNameEntryHighScore();
	DrawNameEntryCurrentScore();
	DrawNameEntryName();
	DrawNameEntryScoreName();
	DrawNameEntryGuide();
	if (nameEntry.state == NAMEENTRY_INPUT)
	{
		DrawNameEntryChar();
		DrawNameEntryCursor();
	}
}

/******************************************************************************
文字セット描画処理
******************************************************************************/
void DrawNameEntryChar(void)
{
	for (int i = 0; i < CHARCHIP_MAX; i++)
	{
		SetVertexNameEntryChar(i);
		DrawCharChip(vertexWk, i);
	}
}

/******************************************************************************
背景描画処理
******************************************************************************/
void DrawNameEntryBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αブレンドを行わない
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	pDevice->SetTexture(0, texture[NAMEENTRY_BACK]);

	SetTextureNameEntry();
	SetVertexNameEntryBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//αブレンドを再設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

/******************************************************************************
カーソル描画処理
******************************************************************************/
void DrawNameEntryCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[NAMEENTRY_CURSOR]);

	SetVertexNameEntryCursor(nameEntry.selectedChar);

	SetTextureNameEntryCursor();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}

/******************************************************************************
プレイヤー名描画処理
******************************************************************************/
void DrawNameEntryName(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	NAMEENTRY *ptr = &nameEntry;

	for (int i = 0; i < PLAYERNAME_MAX; i++)
	{
		SetVertexNameEntryName(i);

		//if (i == ptr->wordCnt && ptr->selectedChar != CHARCHIP_END && ptr->wordCnt < PLAYERNAME_MAX - 1)
		if (isDrawSelectedChar(i))
		{
			DrawCharChip(vertexWk, ptr->selectedChar);
		}
		else
		{
			DrawCharChip(vertexWk, ptr->nameBuff[i]);
		}
	}

}

/******************************************************************************
選択中の文字とプレイヤー名のどっちを描画するか
******************************************************************************/
bool isDrawSelectedChar(int i)
{
	NAMEENTRY *ptr = &nameEntry;

	if (i != ptr->wordCnt)
	{
		return false;
	}

	if (ptr->selectedChar == CHARCHIP_END)
	{
		return false;
	}

	return true;
}

/******************************************************************************
ハイスコア描画処理
******************************************************************************/
void DrawNameEntryHighScore(void)
{
	int digit, score, num;

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		score = buffHighScore[i];
		digit = (score > 0) ? (int)log10((double)score) + 1 : 1;
		for (int j = 0; j < digit; j++, score /= 10)
		{
			num = score % 10;
			SetVertexNameEntryScore(NAMEENTRY_HIGHSCORE_OFFSET_X * j, NAMEENTRY_HIGHSCORE_OFFSET_Y * i);
			DrawYellowNum(vertexWk, num);
		}
	}
}

/******************************************************************************
ハイスコアネーム描画処理
******************************************************************************/
void DrawNameEntryScoreName(void)
{
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		for (int j = 0; j < PLAYERNAME_MAX; j++)
		{
			if (buffHighScoreName[i][j] == CHARCHIP_NONE)
			{
				break;
			}

			SetVertexNameEntryScoreName(-NAMEENTRY_HIGHSCORE_OFFSET_X * j, NAMEENTRY_HIGHSCORE_OFFSET_Y * i);
			DrawCharChip(vertexWk, buffHighScoreName[i][j]);
		}
	}
}

/******************************************************************************
今回のスコア描画処理
******************************************************************************/
void DrawNameEntryCurrentScore(void)
{
	int score, digit, num;
	SCOREDATA *data = GetScoreDataAdr();

	score = data->score;
	digit = (score > 0) ? (int)log10((double)score) + 1 : 1;

	for (int i = 0; i < digit; i++, score /= 10)
	{
		num = score % 10;
		SetVertexNameEntryCurrentScore(NAMEENTRY_HIGHSCORE_OFFSET_X * i);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
ガイド描画処理
******************************************************************************/
void DrawNameEntryGuide(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetVertexNameEntryGuide();
	SetTextureNameEntryGuide();
	pDevice->SetTexture(0, texture[NAMEENTRY_GUIDE]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexNameEntry(void)
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
void SetTextureNameEntry(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定(カーソル用)
******************************************************************************/
void SetTextureNameEntryCursor(void)
{
	int pattern = nameEntry.cntFrame % NAMEENTRY_CURSORTEX_ANIMAX;

	// テクスチャ座標の設定
	int x = pattern % NAMEENTRY_CURSORTEX_DIVIDE_X;
	int y = pattern / NAMEENTRY_CURSORTEX_DIVIDE_X;
	float sizeX = 1.0f / NAMEENTRY_CURSORTEX_DIVIDE_X;
	float sizeY = 1.0f / NAMEENTRY_CURSORTEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定(ガイド用)
******************************************************************************/
void SetTextureNameEntryGuide(void)
{
	int pattern = nameEntry.state;

	// テクスチャ座標の設定
	int y = pattern;
	float sizeY = 1.0f / NAMEENTRY_GUIDETEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定(入力文字用)
******************************************************************************/
void SetVertexNameEntryChar(int charID)
{
	float posX = NAMEENTRY_CHAR_INITPOS_X + (charID % CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_X;
	float posY = NAMEENTRY_CHAR_INITPOS_Y + (charID / CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_Y;
	NAMEENTRY *ptr = &nameEntry;

	// 頂点座標の設定
	vertexWk[0].vtx.x = posX - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleChar) * ptr->radiusChar;
}

/******************************************************************************
頂点座標の設定（カーソル用）
******************************************************************************/
void SetVertexNameEntryCursor(int charID)
{
	float posX = NAMEENTRY_CHAR_INITPOS_X + (charID % CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_X;
	float posY = NAMEENTRY_CHAR_INITPOS_Y + (charID / CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_Y;

	NAMEENTRY *ptr = &nameEntry;

	// 頂点座標の設定
	vertexWk[0].vtx.x = posX - cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleCursor) * ptr->radiusCursor;
}

/******************************************************************************
頂点座標の設定（プレイヤー名用）
******************************************************************************/
void SetVertexNameEntryName(int wordCnt)
{
	float posX = NAMEENTRY_NAME_INITPOS_X + wordCnt * NAMEENTRY_NAME_OFFSETPOS;
	float posY = NAMEENTRY_NAME_INITPOS_Y;

	NAMEENTRY *ptr = &nameEntry;

	// 頂点座標の設定
	vertexWk[0].vtx.x = posX - cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleName) * ptr->radiusName;
}

/******************************************************************************
頂点座標の設定（背景用）
******************************************************************************/
void SetVertexNameEntryBG(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

/******************************************************************************
頂点座標の設定（スコア用）
******************************************************************************/
void SetVertexNameEntryScore(float offsetX, float offsetY)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_HIGHSCORE_INITPOS_X, NAMEENTRY_HIGHSCORE_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
頂点座標の設定（ハイスコアプレイヤー名用）
******************************************************************************/
void SetVertexNameEntryScoreName(float offsetX, float offsetY)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_SCORENAME_INITPOS_X, NAMEENTRY_SCORENAME_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
頂点座標の設定（スコア用）
******************************************************************************/
void SetVertexNameEntryCurrentScore(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_CURRENTSCORE_INITPOS_X, NAMEENTRY_CURRENTSCORE_INITPOS_Y, 0.0f);
	pos.x += offset;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
頂点座標の設定（ガイド用）
******************************************************************************/
void SetVertexNameEntryGuide(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, NAMEENTRY_GUIDE_INITPOS_Y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(NAMEENTRY_GUIDETEX_SIZE_X, NAMEENTRY_GUIDE_INITPOS_Y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, NAMEENTRY_GUIDE_INITPOS_Y + NAMEENTRY_GUIDETEX_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(NAMEENTRY_GUIDETEX_SIZE_X, NAMEENTRY_GUIDE_INITPOS_Y + NAMEENTRY_GUIDETEX_SIZE_Y, 0.0f);
}

/******************************************************************************
//関数名	：NAMEENTRY *GetNameEntryAdr(int pno)
//引数		：int pno：取得したいネームエントリーのインデックス
//戻り値	：正常な引数：ネームエントリーのアドレス	異常な引数：NULL
//説明		：ネームエントリーのアドレス取得関数
******************************************************************************/
NAMEENTRY *GetNameEntryAdr(void)
{

	return &nameEntry;

}

/******************************************************************************
//関数名	：void CheckUpdateRanking(void)
//引数		：void
//戻り値	：void
//説明		：ランキングの更新
******************************************************************************/
void CheckUpdateRanking(void)
{
	SCOREDATA *data = GetScoreDataAdr();
	
	//ランキングを更新
	CheckHiscoreUpdate(&nameEntry.nameBuff[0]);

	//更新したランキングを描画用配列にコピー
	ArrayCopy(&buffHighScore[0], &data->HiScore[0], HISCORE_MAX);

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		ArrayCopy(&buffHighScoreName[i][0], &data->playerName[i][0], PLAYERNAME_MAX);
	}
}