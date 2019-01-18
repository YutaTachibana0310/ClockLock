//=============================================================================
//
// エネミーパーティクル処理 [enemyParticle.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyParticle.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ENEMYPARTICLE_LIFEFRAME_MAX			(60)
#define ENEMYPARTICLE_LIFEFRAME_MIN			(30)

#define ENEMYPARTICLE_SPEED_MAX				(15.0f)
#define ENEMYPARTICLE_SPEED_MIN				(1.0f)

#define ENEMYPARTICLE_SCALE_MAX				(1.2f)
#define ENEMYPARTICLE_SCALE_MIN				(0.5f)

#define ENEMYPARTICLE_ANGLE_MIN				(60.0f)
#define ENEMYPARTICLE_ANGLE_MAX				(120.0f)


/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnemyParticle(void);				//頂点作成関数
void SetTextureEnemyParticle(void);		// テクスチャ座標の計算処理
void SetVertexEnemyParticle(ENEMYPARTICLE *ptr);	// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

ENEMYPARTICLE enemyParticle[ENEMYPARTICLE_MAX];				//エネミーパーティクル配列
EMITTER_ENEMYPARTICLE	emitterEnemyParticle[ENEMYPARTICLE_EMITTER_MAX];

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnemyParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYPARTICLE* ptr = GetEnemyParticleAdr(0);
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];
	int i;

	for (i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYPARTICLE_INITPOS_X, ENEMYPARTICLE_INITPOS_Y, 0.0f);

		ptr->angle = atan2f(ENEMYPARTICLE_TEXTURE_SIZE_Y, ENEMYPARTICLE_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(ENEMYPARTICLE_TEXTURE_SIZE_X, ENEMYPARTICLE_TEXTURE_SIZE_Y));

		ptr->frameCnt = 0;
		ptr->speed = 0;
		ptr->dir = VECTOR3_ZERO;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	for (i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		emitter->pos = VECTOR3_ZERO;
		emitter->active = false;
		emitter->frameCnt = 0;
		emitter->duration = 0;
	}

	// 頂点情報の作成
	MakeVertexEnemyParticle();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)ENEMYPARTICLE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnemyParticle(void)
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
void UpdateEnemyParticle(void)
{
	ENEMYPARTICLE *ptr = &enemyParticle[0];
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];

	//エミッタの更新
	for (int i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
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
			SetEnemyParticle(emitter->pos, emitter->shape);

		}
	}

	//パーティクルの更新
 	for (int i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
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
void DrawEnemyParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYPARTICLE *ptr = GetEnemyParticleAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エネミーパーティクルを加算合成で描画
	for (i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEnemyParticle(ptr);

			// テクスチャ座標を設定
			SetTextureEnemyParticle();

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnemyParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureEnemyParticle(void)
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
void SetVertexEnemyParticle(ENEMYPARTICLE *ptr)
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
//関数名	：ENEMYPARTICLE *GetEnemyParticleAdr(int pno)
//引数		：int pno：取得したいエネミーパーティクルのインデックス
//戻り値	：正常な引数：エネミーパーティクルのアドレス	異常な引数：NULL
//説明		：エネミーパーティクルのアドレス取得関数
******************************************************************************/
ENEMYPARTICLE *GetEnemyParticleAdr(int pno)
{
	if (pno < ENEMYPARTICLE_MAX)
	{
		return &enemyParticle[pno];
	}
	else
	{
		return NULL;
	}
}

void SetEnemyParticle(D3DXVECTOR3 pos, int shape)
{
	ENEMYPARTICLE *ptr = &enemyParticle[0];

	for (int i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;

		float angle;
		if (shape == EMITTER_CONE)
		{
			angle = RandomRange(ENEMYPARTICLE_ANGLE_MIN, ENEMYPARTICLE_ANGLE_MAX);
		}
		else if (shape == EMITTER_CIRCLE)
		{
			angle = RandomRange(0.0f, 360.0f);
		}

		ptr->dir = D3DXVECTOR3(cosf(RADIAN(angle)), sinf(RADIAN(angle)), 0.0f);

		ptr->initScale = ptr->scale = RandomRange(ENEMYPARTICLE_SCALE_MIN, ENEMYPARTICLE_SCALE_MAX);
		ptr->initSpeed = ptr->speed = RandomRange(ENEMYPARTICLE_SPEED_MIN, ENEMYPARTICLE_SPEED_MAX);

		ptr->lifeFrame = ENEMYPARTICLE_LIFEFRAME_MIN + rand() % ENEMYPARTICLE_LIFEFRAME_MAX;
		ptr->frameCnt = 0;

		ptr->active = true;
		return;
	}
}

void SetEmitterEnemyParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape)
{
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];

	for (int i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (emitter->active)
		{
			continue;
		}

		emitter->pos = pos;
		emitter->frameCnt = 0;
		emitter->shape = shape;
		emitter->duration = duration;
		emitter->emitNum = emitNum;

		emitter->active = true;
		return;
	}
}