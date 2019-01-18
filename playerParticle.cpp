//=============================================================================
//
// エネミーパーティクル処理 [playerParticle.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "playerParticle.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYERPARTICLE_LIFEFRAME_MAX			(30)
#define PLAYERPARTICLE_LIFEFRAME_MIN			(10)

#define PLAYERPARTICLE_SPEED_MAX				(30.0f)
#define PLAYERPARTICLE_SPEED_MIN				(10.0f)

#define PLAYERPARTICLE_SCALE_MIN				(0.5f)

#define PLAYERPARTICLE_ANGLE_MIN				(-10.0f)
#define PLAYERPARTICLE_ANGLE_MAX				(190.0f)

#define PLAYEREXPLOSION_EMITTER_DURATIONMAX		(10)


/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerParticle(void);				//頂点作成関数
void SetTexturePlayerParticle(void);		// テクスチャ座標の計算処理
void SetVertexPlayerParticle(PLAYERPARTICLE *ptr);	// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

PLAYERPARTICLE playerParticle[PLAYERPARTICLE_MAX];				//エネミーパーティクル配列
EMITTER_PLAYERPARTICLE	emitterPlayerParticle[PLAYERPARTICLE_EMITTER_MAX];

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERPARTICLE* ptr = GetPlayerParticleAdr(0);
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];
	int i;

	for (i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERPARTICLE_INITPOS_X, PLAYERPARTICLE_INITPOS_Y, 0.0f);

		ptr->angle = atan2f(PLAYERPARTICLE_TEXTURE_SIZE_Y, PLAYERPARTICLE_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERPARTICLE_TEXTURE_SIZE_X, PLAYERPARTICLE_TEXTURE_SIZE_Y));

		ptr->frameCnt = 0;
		ptr->speed = 0;
		ptr->dir = VECTOR3_ZERO;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	for (i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		emitter->pos = VECTOR3_ZERO;
		emitter->active = false;
		emitter->frameCnt = 0;
		emitter->duration = 0;
	}

	// 頂点情報の作成
	MakeVertexPlayerParticle();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PLAYERPARTICLE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerParticle(void)
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
void UpdatePlayerParticle(void)
{
	PLAYERPARTICLE *ptr = &playerParticle[0];
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];

	//エミッタの更新
	for (int i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (!emitter->active)
		{
			continue;
		}

		if (++emitter->frameCnt == emitter->duration)
		{
			emitter->active = false;
			continue;
		}

		for (int j = 0; j < emitter->emitNum; j++)
		{
			SetPlayerParticle(emitter->pos, emitter->shape, emitter->maxScale);
		}
	}

	//パーティクルの更新
 	for (int i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		//非アクティブであれあコンティニュー
		if (!ptr->active)
		{
			continue;
		}

		//寿命であれば非アクティブにしコンティニュー
		if (++ptr->frameCnt == ptr->lifeFrame)
		{
			ptr->active = false;
			continue;
		}

		//スピードとスケールをイージング処理
		ptr->speed = EaseInCubic((float)ptr->frameCnt, ptr->initSpeed, 0.0f, (float)ptr->lifeFrame);
		ptr->scale = EaseOutCubic((float)ptr->frameCnt, ptr->initScale, 0.0f, (float)ptr->lifeFrame);

		//座標更新
		ptr->pos += ptr->dir * ptr->speed;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERPARTICLE *ptr = GetPlayerParticleAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エネミーパーティクルを加算合成で描画
	for (i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPlayerParticle(ptr);

			// テクスチャ座標を設定
			SetTexturePlayerParticle();

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTexturePlayerParticle(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerParticle(PLAYERPARTICLE *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius * ptr->scale;
}

/******************************************************************************
//関数名	：PLAYERPARTICLE *GetPlayerParticleAdr(int pno)
//引数		：int pno：取得したいエネミーパーティクルのインデックス
//戻り値	：正常な引数：エネミーパーティクルのアドレス	異常な引数：NULL
//説明		：エネミーパーティクルのアドレス取得関数
******************************************************************************/
PLAYERPARTICLE *GetPlayerParticleAdr(int pno)
{
	if (pno < PLAYERPARTICLE_MAX)
	{
		return &playerParticle[pno];
	}
	else
	{
		return NULL;
	}
}

/*
エネミーパーティクルセット関数
*/
void SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale)
{
	PLAYERPARTICLE *ptr = &playerParticle[0];

	for (int i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;

		float angle;
		if (shape == EMITTER_CONE)
		{
			angle = RandomRange(PLAYERPARTICLE_ANGLE_MIN, PLAYERPARTICLE_ANGLE_MAX);
		}
		else if (shape == EMITTER_CIRCLE)
		{
			angle = RandomRange(0.0f, 360.0f);
		}

		ptr->dir = D3DXVECTOR3(cosf(RADIAN(angle)), sinf(RADIAN(angle)), 0.0f);

		ptr->initScale = ptr->scale = RandomRange(PLAYERPARTICLE_SCALE_MIN, maxScale);
		ptr->initSpeed = ptr->speed = RandomRange(PLAYERPARTICLE_SPEED_MIN, PLAYERPARTICLE_SPEED_MAX);

		ptr->lifeFrame = PLAYERPARTICLE_LIFEFRAME_MIN + rand() % PLAYERPARTICLE_LIFEFRAME_MAX;
		ptr->frameCnt = 0;

		ptr->active = true;
		return;
	}
}

/******************************************************************************
//関数名	：void SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale)
//引数1		：D3DXVECTOR3 pos	：発生座標
//引数2		：int shape			：発生形状
//引数3		：float maxScale	：最大スケール
//戻り値	：void
//説明		：エネミーパーティクルのエミッタセット関数
******************************************************************************/
void SetEmitterPlayerParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape, float maxScale)
{
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];

	for (int i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (emitter->active)
		{
			continue;
		}

		emitter->pos = pos;
		emitter->frameCnt = 0;
		emitter->duration = duration;
		emitter->shape = shape;
		emitter->emitNum = emitNum;
		emitter->maxScale = maxScale;

		emitter->active = true;
		return;
	}
}
