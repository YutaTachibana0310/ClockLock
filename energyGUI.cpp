//=============================================================================
//
// 処理 [energyGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "player.h"
#include "energyGUI.h"
#include "myLibrary.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ENERGYGUI_NUM_LOOPMAX				(3)		//数字描画処理のループ最大数

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnergyGUI(void);				//頂点作成関数
void SetTextureEnergyGUI(void);					//テクスチャ座標の計算処理
void SetTextureEnergyGUIGauge(void);			//テクスチャ座標の計算処理（ゲージ用）
void SetVertexEnergyGUI(void);					//頂点の計算処理
void SetVertexEnergyGUIGauge(void);				//頂点の計算処理（ゲージ用）
void SetVertexEnergyGUINum(float offset);		//頂点の計算処理（数字用）			

/*****************************************************************************
構造体定義
*****************************************************************************/
enum TEX_INDEX
{
	BACK,
	GAUGE,
	FRAME,
	TEX_MAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[TEX_MAX];				// テクスチャへのポインタ
static VERTEX_2D		vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, angleNum;									//ポリゴンの角度
static float radius, radiusNum;									//ポリゴンの半径
static D3DXVECTOR3 basePos;										//基準座標

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnergyGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEnergyGUI();

	//基準座標を設定
	basePos = D3DXVECTOR3(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y, 0.0f);

	//各パラメータを計算
	angle = atan2f(ENERGYGUI_BACKTEX_SIZE_Y, ENERGYGUI_BACKTEX_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ENERGYGUI_BACKTEX_SIZE_X, ENERGYGUI_BACKTEX_SIZE_Y));

	angleNum = atan2f(ENERGYGUI_NUMTEX_SIZE_Y, ENERGYGUI_NUMTEX_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(ENERGYGUI_NUMTEX_SIZE_X, ENERGYGUI_NUMTEX_SIZE_Y));

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[BACK] = CreateTextureFromFile((LPSTR)ENERGYGUI_BACKTEX_NAME, pDevice);
		texture[GAUGE] = CreateTextureFromFile((LPSTR)ENERGYGUI_TEXTURE_NAME, pDevice);
		texture[FRAME] = CreateTextureFromFile((LPSTR)ENERGYGUI_FRAMETEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnergyGUI(void)
{
	for (int i = 0; i < TEX_MAX; i++)
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
void UpdateEnergyGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y, 0.0f);

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);

	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnergyGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ゲージを描画
	pDevice->SetTexture(0, texture[GAUGE]);
	SetVertexEnergyGUIGauge();
	SetTextureEnergyGUIGauge();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//ゲージフレームを描画
	pDevice->SetTexture(0, texture[FRAME]);
	SetVertexEnergyGUI();
	SetTextureEnergyGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//数値を描画
	DrawEnergyGUINum();


}

/******************************************************************************
描画処理（背景用）
******************************************************************************/
void DrawEnergyGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[BACK]);

	//頂点座標を設定
	SetVertexEnergyGUI();

	// テクスチャ座標を設定
	SetTextureEnergyGUI();

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（数字用）
******************************************************************************/
void DrawEnergyGUINum(void)
{
	PLAYER *player = GetPlayerAdr(0);

	float ratio = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	int percent = (int)(ratio * 100);
	int digit = (percent != 0) ? (int)log10((double)percent) + 1 : 1;
	
	int num;
	for (int i = 0; i < digit; i++, percent /= 10)
	{
		num = percent % 10;
		SetVertexEnergyGUINum(i * ENERGYGUI_NUM_OFFSET);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnergyGUI(void)
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
テクスチャ座標の設定（背景用）
******************************************************************************/
void SetTextureEnergyGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定(ゲージ用)
******************************************************************************/
void SetTextureEnergyGUIGauge(void)
{
	PLAYER *player = GetPlayerAdr(0);

	float sizeX = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(sizeX, 1.0f);
}

/******************************************************************************
頂点座標の設定（背景用）
******************************************************************************/
void SetVertexEnergyGUI(void)
{
	//static D3DXVECTOR2 pos = D3DXVECTOR2(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y);
	D3DXVECTOR3 pos = basePos;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = pos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = pos.y + sinf(angle) * radius;
}

/******************************************************************************
頂点座標の設定（ゲージ用）
******************************************************************************/
void SetVertexEnergyGUIGauge(void)
{
	//static D3DXVECTOR3 pos = D3DXVECTOR3(ENERGYGUI_GAUGE_INITPOS_X, ENERGYGUI_GAUGE_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ENERGYGUI_GAUGE_INITPOS_X, ENERGYGUI_GAUGE_INITPOS_Y, 0.0f);

	PLAYER *player = GetPlayerAdr(0);
	float sizeX = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	
	vertexWk[0].vtx.x = pos.x;
	vertexWk[0].vtx.y = pos.y;
	vertexWk[1].vtx.x = pos.x + ENERGYGUI_TEXTURE_SIZE_X * sizeX;
	vertexWk[1].vtx.y = pos.y;
	vertexWk[2].vtx.x = pos.x;
	vertexWk[2].vtx.y = pos.y + ENERGYGUI_TEXTURE_SIZE_Y;
	vertexWk[3].vtx.x = pos.x + ENERGYGUI_TEXTURE_SIZE_X * sizeX;
	vertexWk[3].vtx.y = pos.y + ENERGYGUI_TEXTURE_SIZE_Y;
}

/******************************************************************************
頂点座標の設定（数字用）
******************************************************************************/
void SetVertexEnergyGUINum(float offset)
{
	//D3DXVECTOR2 pos = D3DXVECTOR2(ENERGYGUI_NUM_INITPOS_X + offset, ENERGYGUI_NUM_INITPOS_Y);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ENERGYGUI_NUM_INITPOS_X, ENERGYGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum;
}