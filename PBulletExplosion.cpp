//=============================================================================
//
// プレイヤーバレットの爆発処理 [tmplate.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "pBulletExplosion.h"
#include "playerParticle.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PBULLETEXPLOSION_SPEED					(-40.0f)
#define PLAYERPARTICLE_EMITT_NUM				(3)
#define PLAYERPARTICLE_EMITTER_DURATION			(5)
#define PLAYERPARTICLE_SCALE_MAX				(1.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPBulletExplosion(void);								//頂点作成関数
void SetTexturePBulletExplosion(int cntPattern, PBULLETEXPLOSION *ptr);	// テクスチャ座標の計算処理
void SetVertexPBulletExplosion(PBULLETEXPLOSION *ptr);					// 頂点の計算処理

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;							// テクスチャへのポインタ（メイン用）

PBULLETEXPLOSION pBulletExplosion[PBULLETEXPLOSION_MAX];				//プレイヤーバレット配列(メインショット)

static VERTEX_2D		vertexWk[NUM_VERTEX];							//頂点情報格納ワーク

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPBulletExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PBULLETEXPLOSION* ptr = GetPBulletExplosionAdr(0);
	int i;

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PBULLETEXPLOSION_INITPOS_X, PBULLETEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PBULLETEXPLOSION_TEXTURE_SIZE_Y, PBULLETEXPLOSION_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PBULLETEXPLOSION_TEXTURE_SIZE_X, PBULLETEXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexPBulletExplosion();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PBULLETEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPBulletExplosion(void)
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
void UpdatePBulletExplosion(void)
{
	int i;
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//アニメーションの更新
			if (ptr->countAnim % TIMEPBULLETEXPLOSION_ANIMATION == 0)
			{
				ptr->patternAnim++;
				//最後までアニメーションしていたら非アクティブに
				if (ptr->patternAnim >= ANIM_PBULLETEXPLOSION_NUM)
				{
					ptr->active = false;
				}
			}
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPBulletExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//プレイヤーバレットを加算合成で描画
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPBulletExplosion(ptr);

			// テクスチャ座標を設定
			SetTexturePBulletExplosion(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PBULLETEXPLOSION_BB_LEFT, PBULLETEXPLOSION_BB_TOP, 0.0f), PBULLETEXPLOSION_BB_WIDTH, PBULLETEXPLOSION_BB_HIGHT);
#endif
		}
	}
	//描画方法を通常の方法に再設定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPBulletExplosion(void)
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
void SetTexturePBulletExplosion(int cntPattern, PBULLETEXPLOSION *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PBULLETEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPBulletExplosion(PBULLETEXPLOSION *ptr)
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
//関数名	：PBULLETEXPLOSION *GetPBulletExplosionAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（メインショット）
******************************************************************************/
PBULLETEXPLOSION *GetPBulletExplosionAdr(int pno)
{
	if (pno < PBULLETEXPLOSION_MAX)
	{
		return &pBulletExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetPBulletExplosion(D3DXVECTOR3 pos)
//引数		：D3DXVECTOR3 pos	：爆発をセットしたい座標
//戻り値	：void
//説明		：プレイヤーバレットの爆発のセット関数
******************************************************************************/
void SetPBulletExplosion(D3DXVECTOR3 pos)
{
	int i;
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = pos + D3DXVECTOR3(RandomRange(-5.0f, 5.0f), RandomRange(-5.0f, 5.0f), 0.0f);
			ptr->rot.z = RandomRange(0.0f, 360.0f);
			ptr->patternAnim = 0;
			ptr->active = true;
			SetEmitterPlayerParticle(pos, PLAYERPARTICLE_EMITTER_DURATION, PLAYERPARTICLE_EMITT_NUM, EMITTER_CONE, PLAYERPARTICLE_SCALE_MAX);
			return;
		}
	}
}