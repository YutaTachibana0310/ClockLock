//=============================================================================
//
// 処理 [bgObj.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "bgObj.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexBgObj(void);						//頂点作成関数
void SetTextureBgObj(int cntPattern, BGOBJ *ptr);	// テクスチャ座標の計算処理
void SetVertexBgObj(BGOBJ *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

BGOBJ bgObj[BGOBJ_MAX];									//BGオブジェ配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBgObj(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BGOBJ* ptr = GetBgObjAdr(0);
	int i;

	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(BGOBJ_INITPOS_X, BGOBJ_INITPOS_Y - BGOBJ_TEXTURE_SIZE_Y * (i + 1), 0.0f);

		ptr->patternAnim = i;
	}

	// 頂点情報の作成
	MakeVertexBgObj();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)BGOBJ_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBgObj(void)
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
void UpdateBgObj(void)
{
	int i;
	BGOBJ *ptr = GetBgObjAdr(0);

	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		ptr->pos.y += BGOBJ_SPEED;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBgObj(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BGOBJ *ptr = GetBgObjAdr(0);
	int i;
	bool isRendered = false;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//BGオブジェを描画
	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		//画面に映らないオブジェであれば描画を行わない
		isRendered = ptr->pos.y < SCREEN_HEIGHT || ptr->pos.y > -SCREEN_HEIGHT;
		if (isRendered)
		{
			//頂点座標を設定
			SetVertexBgObj(ptr);

			// テクスチャ座標を設定
			SetTextureBgObj(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexBgObj(void)
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
void SetTextureBgObj(int cntPattern, BGOBJ *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % BGOBJ_TEXTURE_DIVIDE_X;
	int y = cntPattern / BGOBJ_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BGOBJ_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BGOBJ_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexBgObj(BGOBJ *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(ptr->pos.x, ptr->pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(ptr->pos.x + BGOBJ_TEXTURE_SIZE_X, ptr->pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(ptr->pos.x, ptr->pos.y + BGOBJ_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(ptr->pos.x + BGOBJ_TEXTURE_SIZE_X, ptr->pos.y + BGOBJ_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
//関数名	：BGOBJ *GetBgObjAdr(int pno)
//引数		：int pno：取得したいBGオブジェのインデックス
//戻り値	：正常な引数：BGオブジェのアドレス	異常な引数：NULL
//説明		：BGオブジェのアドレス取得関数
******************************************************************************/
BGOBJ *GetBgObjAdr(int pno)
{
	if (pno < BGOBJ_MAX)
	{
		return &bgObj[pno];
	}
	else
	{
		return NULL;
	}
}