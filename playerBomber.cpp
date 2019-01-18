//=============================================================================
//
// プレイヤーボンバー処理 [playerBomber.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "playerBomber.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYERBOMBER_ROTANGLE		(-1.0f)

#define PLAYERBOMBER_LIFEFRAME		(180)
#define PLAYERBOMBER_REDUCTIONFRAME	(150)

#define PLAYERBOMBER_SCALEFRAME		(30)
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerBomber(void);				//頂点作成関数
void SetTexturePlayerBomber(int cntPattern);	// テクスチャ座標の計算処理
void SetVertexPlayerBomber(PLAYERBOMBER *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

PLAYERBOMBER playerBomber[PLAYERBOMBER_MAX];				//プレイヤーボンバー配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerBomber(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBOMBER* ptr = GetPlayerBomberAdr(0);
	int i;

	for (i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBOMBER_INITPOS_X, PLAYERBOMBER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBOMBER_TEXTURE_SIZE_Y, PLAYERBOMBER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBOMBER_TEXTURE_SIZE_X, PLAYERBOMBER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexPlayerBomber();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PLAYERBOMBER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerBomber(void)
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
void UpdatePlayerBomber(void)
{
	PLAYERBOMBER *ptr = &playerBomber[0];
	int cnt;

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->countAnim++;

		ptr->rot.z += PLAYERBOMBER_ROTANGLE;

		if (ptr->countAnim > PLAYERBOMBER_REDUCTIONFRAME)
		{
			cnt = ptr->countAnim - PLAYERBOMBER_REDUCTIONFRAME;
			ptr->scale = 1.0f - (float)cnt / PLAYERBOMBER_SCALEFRAME;
		}

		if (ptr->countAnim == PLAYERBOMBER_LIFEFRAME)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerBomber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBOMBER *ptr = GetPlayerBomberAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//プレイヤーボンバーを加算合成で描画
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPlayerBomber(ptr);

			// テクスチャ座標を設定
			SetTexturePlayerBomber(ptr->patternAnim);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBOMBER_BB_LEFT, PLAYERBOMBER_BB_TOP, 0.0f), PLAYERBOMBER_BB_WIDTH, PLAYERBOMBER_BB_HIGHT);
#endif
		}
	}
	//描画方法を通常の方法に再設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerBomber(void)
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
void SetTexturePlayerBomber(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % PLAYERBOMBER_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYERBOMBER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYERBOMBER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYERBOMBER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerBomber(PLAYERBOMBER *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//関数名	：PLAYERBOMBER *GetPlayerBomberAdr(int pno)
//引数		：int pno：取得したいプレイヤーボンバーのインデックス
//戻り値	：正常な引数：プレイヤーボンバーのアドレス	異常な引数：NULL
//説明		：プレイヤーボンバーのアドレス取得関数
******************************************************************************/
PLAYERBOMBER *GetPlayerBomberAdr(int pno)
{
	if (pno < PLAYERBOMBER_MAX)
	{
		return &playerBomber[pno];
	}
	else
	{
		return NULL;
	}
}

bool SetPlayerBomber(D3DXVECTOR3 pos)
{
	PLAYERBOMBER *ptr = &playerBomber[0];

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->countAnim = 0;
		ptr->scale = 1.0f;
		ptr->active = true;
		return true;
	}

	return false;
}