//=============================================================================
//
// エネミーバレットエクスプロージョン処理 [enemyBulletExplosion.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "enemyBulletExplosion.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define EBULLETEXPLOSION_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnemyBulletExplosion(void);				//頂点作成関数
void SetTextureEnemyBulletExplosion(EBULLETEXPLOSION *ptr);	// テクスチャ座標の計算処理
void SetVertexEnemyBulletExplosion(EBULLETEXPLOSION *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum EBULLETEXPLOSION_TEXTYPE
{
	EXPLOSION_GREEN,
	EXPLOSION_ORANGE
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

EBULLETEXPLOSION enemyBulletExplosion[EBULLETEXPLOSION_MAX];				//エネミーバレットエクスプロージョン配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnemyBulletExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EBULLETEXPLOSION* ptr = GetEnemyBulletExplosionAdr(0);
	int i;

	for (i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EBULLETEXPLOSION_INITPOS_X, EBULLETEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(EBULLETEXPLOSION_TEXTURE_SIZE_Y, EBULLETEXPLOSION_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(EBULLETEXPLOSION_TEXTURE_SIZE_X, EBULLETEXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->type = (i < EBULLETEXPLOSION_MAX / 2) ? EXPLOSION_GREEN : EXPLOSION_ORANGE;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEnemyBulletExplosion();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)EBULLETEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnemyBulletExplosion(void)
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
void UpdateEnemyBulletExplosion(void)
{
	EBULLETEXPLOSION *ptr = &enemyBulletExplosion[0];

	for (int i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//アニメーション
		if (++ptr->countAnim % EBULLETEXPLOSION_TIME_ANIMATION == 0)
		{
			ptr->patternAnim++;
			if (ptr->patternAnim == EBULLETEXPLOSION_ANIM_NUM)
			{
				ptr->active = false;
			}
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnemyBulletExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EBULLETEXPLOSION *ptr = GetEnemyBulletExplosionAdr(0);
	int i;

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エネミーバレットエクスプロージョンを加算合成で描画
	for (i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEnemyBulletExplosion(ptr);

			// テクスチャ座標を設定
			SetTextureEnemyBulletExplosion(ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnemyBulletExplosion(void)
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
void SetTextureEnemyBulletExplosion(EBULLETEXPLOSION *ptr)
{
	// テクスチャ座標の設定
	int x = ptr->patternAnim % EBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	int y = ptr->type;
	float sizeX = 1.0f / EBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EBULLETEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEnemyBulletExplosion(EBULLETEXPLOSION *ptr)
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
//関数名	：EBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno)
//引数		：int pno：取得したいエネミーバレットエクスプロージョンのインデックス
//戻り値	：正常な引数：エネミーバレットエクスプロージョンのアドレス	異常な引数：NULL
//説明		：エネミーバレットエクスプロージョンのアドレス取得関数
******************************************************************************/
EBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno)
{
	if (pno < EBULLETEXPLOSION_MAX)
	{
		return &enemyBulletExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type)
//引数1		：D3DXVECTOR3 pos	：セット座標
//引数2		：BULLET_TYPE type	：爆発するバレットのタイプ
//戻り値	：正常な引数：エネミーバレットエクスプロージョンのアドレス	異常な引数：NULL
//説明		：エネミーバレットエクスプロージョンのアドレス取得関数
******************************************************************************/
void SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type)
{
	EBULLETEXPLOSION *ptr = &enemyBulletExplosion[0];

	for (int i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->type = type / ORANGE_ROTATE;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->active = true;
		return;
	}
}
