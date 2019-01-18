//=============================================================================
//
// グランドファイア処理 [groundFire.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "groundFire.h"
#include "bg.h"
#include "bgObj.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexGroundFire(void);					//頂点作成関数
void SetTextureGroundFire(int cntPattern);			// テクスチャ座標の計算処理
void SetVertexGroundFire(GROUNDFIRE *ptr);			// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;			// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
static float radius, angle;							// ポリゴンの半径、角度

GROUNDFIRE groundFire[GROUNDFIRE_MAX];				// グランドファイア配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitGroundFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GROUNDFIRE* ptr = GetGroundFireAdr(0);
	int i;

	radius = D3DXVec2Length(&D3DXVECTOR2(GROUNDFIRE_TEXTURE_SIZE_X, GROUNDFIRE_TEXTURE_SIZE_Y));
	angle = atan2f(GROUNDFIRE_TEXTURE_SIZE_Y, GROUNDFIRE_TEXTURE_SIZE_X);

	for (i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(GROUNDFIRE_INITPOS_X, GROUNDFIRE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexGroundFire();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)GROUNDFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitGroundFire(void)
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
void UpdateGroundFire(void)
{
	GROUNDFIRE *ptr = GetGroundFireAdr(0);

	for (int i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		//非アクティブであればcontinue
		if (!ptr->active)
		{
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % GROUNDFIRE_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, GROUNDFIRE_ANIM_NUM, ptr->patternAnim + 1);
		}

		//座標の更新
		ptr->pos.y += BGOBJ_SPEED;

		//画面外に出ていたら非アクティブに
		if (ptr->pos.y > SCREEN_HEIGHT + GROUNDFIRE_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawGroundFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GROUNDFIRE *ptr = GetGroundFireAdr(0);
	int i;

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//グランドファイアを描画
	for (i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexGroundFire(ptr);

			// テクスチャ座標を設定
			SetTextureGroundFire(ptr->patternAnim);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexGroundFire(void)
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
void SetTextureGroundFire(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % GROUNDFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / GROUNDFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / GROUNDFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / GROUNDFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexGroundFire(GROUNDFIRE *ptr)
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
//関数名	：GROUNDFIRE *GetGroundFireAdr(int pno)
//引数		：int pno：取得したいグランドファイアのインデックス
//戻り値	：正常な引数：グランドファイアのアドレス	異常な引数：NULL
//説明		：グランドファイアのアドレス取得関数
******************************************************************************/
GROUNDFIRE *GetGroundFireAdr(int pno)
{
	if (pno < GROUNDFIRE_MAX)
	{
		return &groundFire[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
//引数1		：D3DXVECTOR3 pos	：セット座標
//引数2		：D3DXVECTOR3 rot	：回転量
//戻り値	：void
//説明		：グランドファイアのセット関数
******************************************************************************/
void SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	GROUNDFIRE *ptr = &groundFire[0];

	for (int i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->rot = rot;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = true;
		return;
	}
}