//=============================================================================
//
// エナジーアイテム処理 [energyItem.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "energyItem.h"
#include "bg.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ENERGYITEM_SPEED			(2.5f)

#define ENERGYITEM_SHADOW_OFFSET	(D3DXVECTOR3(50.0f, 100.0f, 0.0f))
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnergyItem(void);					//頂点作成関数
void SetTextureEnergyItem(int cntPattern);			// テクスチャ座標の計算処理
void SetVertexEnergyItem(ENERGYITEM *ptr);			// 頂点の計算処理
void SetShadowEnergyItem(ENERGYITEM *ptr);			//影の頂点設定

/*****************************************************************************
構造体定義
*****************************************************************************/
enum ENERGYITEM_TEX
{
	ENERGYITEM_SHADOW,
	ENERGYITEM_BODY,
	ENERGYITEM_TEXMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[ENERGYITEM_TEXMAX];	// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, radius;								//ポリゴンの半径、角度

ENERGYITEM energyItem[ENERGYITEM_MAX];					//エナジーアイテム配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnergyItem(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENERGYITEM* ptr = GetEnergyItemAdr(0);
	int i;


	angle = atan2f(ENERGYITEM_TEXTURE_SIZE_Y, ENERGYITEM_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ENERGYITEM_TEXTURE_SIZE_X, ENERGYITEM_TEXTURE_SIZE_Y));

	for (i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENERGYITEM_INITPOS_X, ENERGYITEM_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->state = ENERGYITEM_DEFAULT;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEnergyItem();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture[ENERGYITEM_SHADOW] = CreateTextureFromFile((LPSTR)ENERGYITEM_SHADOWTEX_NAME, pDevice);
		texture[ENERGYITEM_BODY] = CreateTextureFromFile((LPSTR)ENERGYITEM_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnergyItem(void)
{
	for (int i = 0; i < ENERGYITEM_TEXMAX; i++)
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
void UpdateEnergyItem(void)
{
	ENERGYITEM *ptr = GetEnergyItemAdr(0);

	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//座標を更新
		ptr->pos += ptr->moveDir * ENERGYITEM_SPEED;

		//画面外判定
		if (ptr->pos.x > BG_RIGHT + ENERGYITEM_TEXTURE_SIZE_X || ptr->pos.x < BG_LEFT + ENERGYITEM_TEXTURE_SIZE_X)
		{
			ptr->active = false;
		}
		else if (ptr->pos.y > SCREEN_HEIGHT + ENERGYITEM_TEXTURE_SIZE_Y || ptr->pos.y < -ENERGYITEM_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % ENERGYITEM_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, ENERGYITEM_ANIM_NUM, ptr->patternAnim + 1);
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnergyItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENERGYITEM *ptr = GetEnergyItemAdr(0);
	int i;

	// 影を描画
	pDevice->SetTexture(0, texture[ENERGYITEM_SHADOW]);
	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			SetShadowEnergyItem(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//エナジーアイテムを描画
	pDevice->SetTexture(0, texture[ENERGYITEM_BODY]);
	ptr = GetEnergyItemAdr(0);
	for (i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEnergyItem(ptr);

			// テクスチャ座標を設定
			SetTextureEnergyItem(ptr->patternAnim);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(ENERGYITEM_BB_LEFT, ENERGYITEM_BB_TOP, 0.0f), ENERGYITEM_BB_WIDTH, ENERGYITEM_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnergyItem(void)
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
void SetTextureEnergyItem(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % ENERGYITEM_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENERGYITEM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENERGYITEM_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENERGYITEM_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEnergyItem(ENERGYITEM *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;
}

/******************************************************************************
影の頂点の設定
******************************************************************************/
void SetShadowEnergyItem(ENERGYITEM *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;

	//オフセット
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += ENERGYITEM_SHADOW_OFFSET;
	}

	//テクスチャ設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//関数名	：ENERGYITEM *GetEnergyItemAdr(int pno)
//引数		：int pno：取得したいエナジーアイテムのインデックス
//戻り値	：正常な引数：エナジーアイテムのアドレス	異常な引数：NULL
//説明		：エナジーアイテムのアドレス取得関数
******************************************************************************/
ENERGYITEM *GetEnergyItemAdr(int pno)
{
	if (pno < ENERGYITEM_MAX)
	{
		return &energyItem[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetEnergyItem(D3DXVECTOR3 pos)
//引数		：D3DXVECTOR3 pos		：出現座標
//戻り値	：void
//説明		：エナジーアイテムのセット関数
******************************************************************************/
void SetEnergyItem(D3DXVECTOR3 pos)
{
	ENERGYITEM *ptr = GetEnergyItemAdr(0);

	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->moveDir = -VECTOR3_UP;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->active = true;
		return;
	}
}