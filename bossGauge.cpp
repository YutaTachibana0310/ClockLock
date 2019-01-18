//=============================================================================
//
// ボスゲージ処理 [bossGauge.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bossGauge.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BOSSGAUGE_OPEN_VALUE				(0.01f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexBossGauge(void);				//頂点作成関数
void SetTextureBossGauge(BOSSGAUGE *ptr);		// テクスチャ座標の計算処理
void SetTextureBossGaugeFrame(void);
void SetVertexBossGauge(BOSSGAUGE *ptr);		// 頂点の計算処理
void SetVertexBossGaugeFrame(BOSSGAUGE *ptr);		

/*****************************************************************************
構造体定義
*****************************************************************************/
enum BOSSGAUGE_TEXTURE
{
	BOSSGAUGE_FRAME,
	BOSSGAUGE_GAUGE
};

enum BOSSGAUGE_STATE
{
	BOSSGAUGE_OPEN,
	BOSSGAUGE_CLOSE,
	BOSSGAUGE_DEFAULT
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

BOSSGAUGE bossGauge;						//ボスゲージ構造体

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBossGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOSSGAUGE* ptr = GetBossGaugeAdr();

	ptr->pos = D3DXVECTOR3(BOSSGAUGE_INITPOS_X, BOSSGAUGE_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(BOSSGAUGE_TEXTURE_SIZE_Y, BOSSGAUGE_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(BOSSGAUGE_TEXTURE_SIZE_X, BOSSGAUGE_TEXTURE_SIZE_Y));

	ptr->state = BOSSGAUGE_DEFAULT;
	ptr->scale = 0.0f;

	ptr->active = false;

	// 頂点情報の作成
	MakeVertexBossGauge();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)BOSSGAUGE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBossGauge(void)
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
void UpdateBossGauge(void)
{
	BOSSGAUGE *ptr = &bossGauge;

	switch(ptr->state)
	{
	case BOSSGAUGE_OPEN:
		ptr->scale = Clampf(0.0f, 1.0f, ptr->scale + BOSSGAUGE_OPEN_VALUE);
		if (ptr->scale == 1.0f)
		{
			ptr->state = BOSSGAUGE_DEFAULT;
		}
		break;

	case BOSSGAUGE_CLOSE:
		ptr->scale = Clampf(0.0f, 1.0f, ptr->scale + BOSSGAUGE_OPEN_VALUE);
		if (ptr->scale == 0.0f)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOSSGAUGE *ptr = GetBossGaugeAdr();

	if (!ptr->active)
	{
		return;
	}

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//ボスゲージを描画
	SetVertexBossGauge(ptr);
	SetTextureBossGauge(ptr);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//フレームを描画
	SetVertexBossGaugeFrame(ptr);
	SetTextureBossGaugeFrame();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexBossGauge(void)
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
void SetTextureBossGauge(BOSSGAUGE *ptr)
{
	// テクスチャ座標の設定
	int x = BOSSGAUGE_GAUGE % BOSSGAUGE_TEXTURE_DIVIDE_X;
	int y = BOSSGAUGE_GAUGE / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_Y;
	
	if (ptr->state == BOSSGAUGE_DEFAULT)
	{
		vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->percent, (float)(y)* sizeY);
		vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->percent, (float)(y)* sizeY + sizeY);
	}
	else
	{
		vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}

/******************************************************************************
テクスチャ座標の設定(フレーム用)
******************************************************************************/
void SetTextureBossGaugeFrame(void)
{
	// テクスチャ座標の設定
	int x = BOSSGAUGE_FRAME % BOSSGAUGE_TEXTURE_DIVIDE_X;
	int y = BOSSGAUGE_FRAME / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexBossGauge(BOSSGAUGE *ptr)
{
	// 頂点座標の設定
	if (ptr->state == BOSSGAUGE_DEFAULT)
	{
		vertexWk[0].vtx.x = vertexWk[1].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[0].vtx.y = vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.x = vertexWk[3].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.y = vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;

		vertexWk[1].vtx.x += BOSSGAUGE_TEXTURE_SIZE_X * 2 * ptr->percent;
		vertexWk[3].vtx.x += BOSSGAUGE_TEXTURE_SIZE_X * 2 * ptr->percent;
	}
	else
	{
		vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	}

}

/******************************************************************************
頂点座標の設定(フレーム用)
******************************************************************************/
void SetVertexBossGaugeFrame(BOSSGAUGE *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}


/******************************************************************************
//関数名	：BOSSGAUGE *GetBossGaugeAdr(int pno)
//引数		：int pno：取得したいボスゲージのインデックス
//戻り値	：正常な引数：ボスゲージのアドレス	異常な引数：NULL
//説明		：ボスゲージのアドレス取得関数
******************************************************************************/
BOSSGAUGE *GetBossGaugeAdr(void)
{
	return &bossGauge;
}

/******************************************************************************
//関数名	：BOSSGAUGE *SetBossGauge(void)
//引数		：void
//戻り値	：セットしたゲージのアドレス
//説明		：ボスゲージのset関数
******************************************************************************/
BOSSGAUGE *SetBossGauge(void)
{
	BOSSGAUGE *ptr = &bossGauge;

	ptr->active = true;
	ptr->state = BOSSGAUGE_OPEN;
	ptr->scale = 0.0f;
	ptr->percent = 1.0f;

	return ptr;
}