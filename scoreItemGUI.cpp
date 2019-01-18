//=============================================================================
//
// 処理 [scoreItemGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "scoreItem.h"
#include "scoreItemGUI.h"
#include "GUI.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SCOREITEMGUI_SCALE_MAX			(1.5f)		//スケールの最大値
#define SCOREITEMGUI_SCALE_MIN			(1.0f)		//スケールの最小値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexScoreItemGUI(void);				//頂点作成関数
void SetTextureScoreItemGUI(void);					// テクスチャ座標の計算処理（背景用）
void SetTextureScoreItemNum(int cntPattern);		// テクスチャ座標の計算処理（数字用）
void SetVertexScoreItemGUI(void);					// 頂点の計算処理（背景用）
void SetVertexScoreItemNum(float offset);			// 頂点の計算処理（数字用）
/*****************************************************************************
構造体定義
*****************************************************************************/
enum TEXTURE
{
	BACK,
	NUM
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SCOREITEMGUI_TEXTURE_NUM]; // テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
//static D3DXVECTOR3 posPolygon;							//ポリゴンの座標
static float angleNum, radiusNum, scale;				//ポリゴンの角度、半径、スケール（数字用）
static float angle, radius;								//ポリゴンの角度、半径（背景用）
static D3DXVECTOR3 basePos;								//基準座標

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScoreItemGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	angle = atan2f(SCOREITEMGUI_TEXTURE_SIZE_Y, SCOREITEMGUI_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SCOREITEMGUI_TEXTURE_SIZE_X, SCOREITEMGUI_TEXTURE_SIZE_Y));
	angleNum = atan2f(SCOREITEMGUI_NUMTEX_SIZE_Y, SCOREITEMGUI_NUMTEX_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(SCOREITEMGUI_NUMTEX_SIZE_X, SCOREITEMGUI_NUMTEX_SIZE_Y));
	scale = 1.0f;

	//基準座標の設定
	basePos = D3DXVECTOR3(SCOREITEMGUI_INITPOS_X, SCOREITEMGUI_INITPOS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexScoreItemGUI();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture[BACK] = CreateTextureFromFile((LPSTR)SCOREITEMGUI_TEXTURE_NAME, pDevice);
		texture[NUM] = CreateTextureFromFile((LPSTR)SCOREITEMGUI_NUMTEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScoreItemGUI(void)
{
	for (int i = 0; i < SCOREITEMGUI_TEXTURE_NUM; i++)
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
void UpdateScoreItemGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(SCOREITEMGUI_INITPOS_X, SCOREITEMGUI_INITPOS_Y, 0.0f);

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}
}

/******************************************************************************
描画処理(数字用)
******************************************************************************/
void DrawScoreItemGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, data, digitMax;
	int cntItem = GetScoreItemCount();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//数字を描画
	pDevice->SetTexture(0, texture[NUM]);
	digitMax = (cntItem > 0) ? (int)log10((double)cntItem) + 1 : 1;
	for (i = 0; i < digitMax; i++, cntItem /= 10)
	{
		data = cntItem % 10;
		SetTextureScoreItemNum(data);
		SetVertexScoreItemNum(SCOREITEMGUI_NUM_OFFSETPOS * i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
描画処理(背景用)
******************************************************************************/
void DrawScoreItemGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUI背景を描画
	pDevice->SetTexture(0, texture[BACK]);
	SetTextureScoreItemGUI();
	SetVertexScoreItemGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexScoreItemGUI(void)
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
テクスチャ座標の設定(背景用)
******************************************************************************/
void SetTextureScoreItemGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定(数字用)
******************************************************************************/
void SetTextureScoreItemNum(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % SCOREITEMGUI_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCOREITEMGUI_TEXTURE_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
頂点座標の設定(背景用)
******************************************************************************/
void SetVertexScoreItemGUI(void)
{
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
void SetVertexScoreItemNum(float offset)
{
	// 頂点座標の設定
	float scale = GetScoreItemScale();
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(SCOREITEMGUI_NUM_INITPOS_X, SCOREITEMGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
}