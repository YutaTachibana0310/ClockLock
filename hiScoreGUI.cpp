//=============================================================================
//
// スコア処理 [score.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "score.h"
#include "hiScoreGUI.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexHiScoreGUI(void);							//頂点作成関数
void SetTextureHiScoreGUI(void);								//テクスチャ座標の計算処理
void SetVertexHiScoreGUINum(float offsetX, float offsetY);		//頂点の計算処理(数字用)
void SetVertexHiScoreGUIBack(void);							//頂点の計算処理（背景用）

enum HISCORE_TEXTURE
{
	TEXTURE_BACK,
	TEXTURE_MAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[TEXTURE_MAX];				// スコアテクスチャへのポインタ

static VERTEX_2D				vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

static float					radiusNum, radiusBack;				//ポリゴンの頂点への長さ
static float					angleNum, angleBack;				//ポリゴンの頂点への角度
static float					scale;								//ポリゴンのスケール
static int						scoreBuff[HISCORE_DRAW_MAX];		//描画するスコアの配列
static D3DXVECTOR3				basePos;							//基準座標
//static int					state;								//描画ステート

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitHiScoreGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	scale = 1.0f;

	//各パラメータの初期化
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(HISCORENUM_TEXTURE_SIZE_X, HISCORENUM_TEXTURE_SIZE_Y));
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(HISCOREBACK_TEXTURE_SIZE_X, HISCOREBACK_TEXTURE_SIZE_Y));

	angleNum = atan2f(HISCORENUM_TEXTURE_SIZE_Y, HISCORENUM_TEXTURE_SIZE_X);
	angleBack = atan2f(HISCOREBACK_TEXTURE_SIZE_Y, HISCOREBACK_TEXTURE_SIZE_X);

	//基準座標の設定
	basePos = D3DXVECTOR3(HISCORE_INITPOS_X, HISCORE_INITPOS_Y, 0.0f);
	
	// 頂点情報の作成
	MakeVertexHiScoreGUI();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[TEXTURE_BACK] = CreateTextureFromFile((LPSTR)HISCORE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitHiScoreGUI(void)
{
	int i;
	for (i = 0; i < TEXTURE_MAX; i++)
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
void UpdateHiScoreGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(HISCORE_INITPOS_X, HISCORE_INITPOS_Y, 0.0f);

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}

	SCOREDATA *data = GetScoreDataAdr();

	//現在のハイスコアを配列に保存
	for (int i = 0; i < HISCORE_DRAW_MAX; i++)
	{
		scoreBuff[i] = data->HiScore[i];
	}

	//ハイスコアの挿入が必要か確認
	for (int i = 0; i < HISCORE_DRAW_MAX; i++)
	{
		if (data->score == Max(data->score, scoreBuff[i]))
		{
			ArrayShiftDown(&scoreBuff[0], i, HISCORE_DRAW_MAX);
			scoreBuff[i] = data->score;
			break;
		}
	}
}

/******************************************************************************
描画処理(数字用）
******************************************************************************/
void DrawHiScoreGUI(void)
{

}

/******************************************************************************
描画処理(背景用）
******************************************************************************/
void DrawHiScoreGUIBack(void)
{
	PDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//スコア背景の描画
	pDevice->SetTexture(0, texture[TEXTURE_BACK]);
	SetTextureHiScoreGUI();
	SetVertexHiScoreGUIBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理(数字用）
******************************************************************************/
void DrawHiscoreNum(void)
{
	SCOREDATA *scoreData = GetScoreDataAdr();
	int digitMax, hiScore, num;

	for (int i = 0, drawNum = 0; i < HISCORE_DRAW_MAX; i++)
	{
		hiScore = scoreBuff[i];
		digitMax = (hiScore > 0) ? (int)log10((double)hiScore) + 1 : 1;

		for (int j = 0; j < digitMax; j++, hiScore /= 10)
		{
			num = hiScore % 10;
			SetVertexHiScoreGUINum(j * HISCORENUM_OFFSETPOS_X, i * HISCORENUM_OFFSETPOS_Y);
			DrawYellowNum(vertexWk, num);
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexHiScoreGUI(void)
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
void SetTextureHiScoreGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
頂点座標の設定(背景用)
******************************************************************************/
void SetVertexHiScoreGUIBack(void)
{
	// 頂点座標の設定
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
頂点座標の設定(数字用)
******************************************************************************/
void SetVertexHiScoreGUINum(float offsetX, float offsetY)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(HISCORENUM_INITPOS_X + offsetX, HISCORENUM_INITPOS_Y + offsetY, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(HISCORENUM_INITPOS_X, HISCORENUM_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

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