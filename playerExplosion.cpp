//=============================================================================
//
// プレイヤー爆発処理 [playerExplosion.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "playerExplosion.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerExplosion(void);				//頂点作成関数
void SetTexturePlayerExplosion(int cntPattern, PLAYEREXPLOSION *ptr);	// テクスチャ座標の計算処理
void SetVertexPlayerExplosion(PLAYEREXPLOSION *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D		vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

PLAYEREXPLOSION playerExplosion[PLAYEREXPLOSION_MAX];				//プレイヤー爆発配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYEREXPLOSION* ptr = GetPlayerExplosionAdr(0);
	int i;

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYEREXPLOSION_INITPOS_X, PLAYEREXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYEREXPLOSION_TEXTURE_SIZE_Y, PLAYEREXPLOSION_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYEREXPLOSION_TEXTURE_SIZE_X, PLAYEREXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexPlayerExplosion();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PLAYEREXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerExplosion(void)
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
void UpdatePlayerExplosion(void)
{
	int i;
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % PLAYEREXPLOSION_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % PLAYEREXPLOSION_ANIM_NUM;
		}

		//とりあえず非アクティブ処理
		if (ptr->countAnim > 32)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//プレイヤー爆発を描画
	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPlayerExplosion(ptr);

			// テクスチャ座標を設定
			SetTexturePlayerExplosion(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerExplosion(void)
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
void SetTexturePlayerExplosion(int cntPattern, PLAYEREXPLOSION *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYEREXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerExplosion(PLAYEREXPLOSION *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
//関数名	：PLAYEREXPLOSION *GetPlayerExplosionAdr(int pno)
//引数		：int pno：取得したいプレイヤー爆発のインデックス
//戻り値	：正常な引数：プレイヤー爆発のアドレス	異常な引数：NULL
//説明		：プレイヤー爆発のアドレス取得関数
******************************************************************************/
PLAYEREXPLOSION *GetPlayerExplosionAdr(int pno)
{
	if (pno < PLAYEREXPLOSION_MAX)
	{
		return &playerExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetPlayerExplosion(D3DXVECTOR3 pos)
//引数		：D3DXVECTOR3 pos		：セットしたい座標
//戻り値	：void
//説明		：プレイヤー爆発のセット関数
******************************************************************************/
void SetPlayerExplosion(D3DXVECTOR3 pos)
{
	int i;
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->patternAnim = 0;
		ptr->countAnim = 0;
		ptr->active = true;
		return;
	}
}