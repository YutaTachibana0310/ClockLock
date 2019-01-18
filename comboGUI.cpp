//=============================================================================
//
// コンボGUI処理 [comboGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "comboGUI.h"
#include "GUI.h"
#include "stageManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define COMBOGUI_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexComboGUI(void);				//頂点作成関数
void SetTextureComboGUI(void);					// テクスチャ座標の計算処理
void SetVertexComboGUI(void);					// 頂点の計算処理
void SetVertexComboGUINum(float offset);		// 頂点の計算処理（数字用）

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

static float radius;									// ポリゴンの半径
static float angle;										// ポリゴンの角度

static float radiusNum;									// 数字ポリゴンの半径
static float angleNum;									// 数字ポリゴンの角度

static D3DXVECTOR3 basePos;								//基準座標

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitComboGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各パラメータの計算
	angle = atan2f(COMBOGUI_TEXTURE_SIZE_Y, COMBOGUI_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(COMBOGUI_TEXTURE_SIZE_X, COMBOGUI_TEXTURE_SIZE_Y));

	angleNum = atan2f(COMBOGUI_NUM_SIZE_Y, COMBOGUI_NUM_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(COMBOGUI_NUM_SIZE_X, COMBOGUI_NUM_SIZE_Y));

	//基準座標の設定
	basePos = D3DXVECTOR3(COMBOGUI_INITPOS_X, COMBOGUI_INITPOS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexComboGUI();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)COMBOGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitComboGUI(void)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateComboGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(COMBOGUI_INITPOS_X, COMBOGUI_INITPOS_Y, 0.0f);

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
void DrawComboGUI(void)
{
	STAGEMANAGER *manager = GetStageManagerAdr();

	int comboCnt = manager->combo;
	int digitMax = (comboCnt == 0) ? 1 : (int)log10f((float)comboCnt) + 1;
	int num;

	for (int i = 0; i < digitMax; i++, comboCnt /= 10)
	{
		num = comboCnt % 10;
		SetVertexComboGUINum(COMBOGUI_NUM_OFFSET * i);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
描画処理（背景用）
******************************************************************************/
void DrawComboGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);
	SetTextureComboGUI();
	SetVertexComboGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexComboGUI(void)
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
void SetTextureComboGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexComboGUI(void)
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
void SetVertexComboGUINum(float offset)
{
	// 頂点座標の設定
	float scale = GetStageManagerAdr()->comboScale;
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(COMBOGUI_NUM_INITPOS_X, COMBOGUI_NUM_INITPOS_Y, 0.0f);
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