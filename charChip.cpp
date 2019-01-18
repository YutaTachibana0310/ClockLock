//=============================================================================
//
// 文字チップ処理 [charChip.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "charChip.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void SetTextureCharChip(int cntPattern, VERTEX_2D *vertexWk);	// テクスチャ座標の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitCharChip(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)CHARCHIP_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitCharChip(void)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawCharChip(VERTEX_2D *vertexWk, int charchipID)
{
	if (charchipID < CHARCHIP_A || charchipID >= CHARCHIP_MAX)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	// テクスチャ座標を設定
	SetTextureCharChip(charchipID, vertexWk);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureCharChip(int cntPattern, VERTEX_2D *vertexWk)
{
	// テクスチャ座標の設定
	int x = cntPattern % CHARCHIP_TEXTURE_DIVIDE_X;
	int y = cntPattern / CHARCHIP_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / CHARCHIP_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / CHARCHIP_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}