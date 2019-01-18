//=============================================================================
//
// エフェクトエクスプロージョン処理 [effectExplosion.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "effectExplosion.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define EFFECTEXPLOSION_POS_RANGE				(80.0f)			//エフェクトエクスプロージョンの座標設定範囲
#define EFFECTEXPLOSION_INITSPEED				(0.5f)			//初期スピード
#define EFFECTEXPLOSION_SLOWFRAME				(20)			//減速するフレーム
#define EFFECTEXPLOSION_SPEEDMAGNI_MIN			(0.8f)			//速度係数の最小値
#define EFFECTEXPLOSION_SPEEDMAGNI_MAX			(1.5f)			//速度係数の最大値
#define EFFECTEXPLOSION_SCALE_MIN				(0.8f)			//スケール最小値
#define EFFECTEXPLOSION_SCALE_MAX				(1.5f)			//スケール最大値
#define EFFECTEXPLOSION_INITPATTERN	(EFFECTEXPLOSION_ANIM_TIME + rand() % 2);	//アニメパターンの初期値
#define EFFECTEXPLOSION_BASEROTATION			(RADIAN(45.0f))	//回転角度の初期加算値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEffectExplosion(void);						//頂点作成関数
void SetTextureEffectExplosion(int cntPattern, EFFECT *ptr);	// テクスチャ座標の計算処理
void SetVertexEffectExplosion(EFFECT *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float radius, angle;

EFFECT effectExplosion[EFFECTEXPLOSION_MAX];					//エフェクトエクスプロージョン配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEffectExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectExplosionAdr(0);
	int i;

	angle = atan2f(EFFECTEXPLOSION_TEXTURE_SIZE_Y, EFFECTEXPLOSION_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTEXPLOSION_TEXTURE_SIZE_X, EFFECTEXPLOSION_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTEXPLOSION_INITPOS_X, EFFECTEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->animTime = EFFECTEXPLOSION_ANIM_TIME;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEffectExplosion();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)EFFECTEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEffectExplosion(void)
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
void UpdateEffectExplosion(void)
{
	int i;
	EFFECT *ptr = GetEffectExplosionAdr(0);

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		//非アクティブであれば処理しない
		if (!ptr->active)
		{
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % ptr->animTime == 0)
		{
			ptr->patternAnim++;

			//パターンの最後までアニメーションさせていたら非アクティブにセット
			if (ptr->patternAnim >= EFFECTEXPLOSION_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//座標の更新
		ptr->pos += ptr->velocity * ptr->speed;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEffectExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectExplosionAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エフェクトエクスプロージョンを描画
	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEffectExplosion(ptr);

			// テクスチャ座標を設定
			SetTextureEffectExplosion(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEffectExplosion(void)
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
void SetTextureEffectExplosion(int cntPattern, EFFECT *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEffectExplosion(EFFECT *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius * ptr->scale;
}

/******************************************************************************
//関数名	：EFFECTEXPLOSION *GetEffectExplosionAdr(int pno)
//引数		：int pno：取得したいエフェクトエクスプロージョンのインデックス
//戻り値	：正常な引数：エフェクトエクスプロージョンのアドレス	異常な引数：NULL
//説明		：エフェクトエクスプロージョンのアドレス取得関数
******************************************************************************/
EFFECT *GetEffectExplosionAdr(int pno)
{
	if (pno < EFFECTEXPLOSION_MAX)
	{
		return &effectExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：EFFECT *SetEffectExplosion(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos		：設定座標
//引数2		：D3DXVECTOR3 dir		：進む方向
//戻り値	：成功時：セットしたエフェクトへのポインタ	失敗時：NULL
//説明		：エフェクトエクスプロージョンのセット関数
******************************************************************************/
EFFECT *SetEffectExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectExplosionAdr(0);
	float tmp = RADIAN(RandomRange(0.0f, 360.0f));

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		//未使用のエフェクトエクスプロージョンがあればセット
		if (!ptr->active)
		{
			//座標の設定
			ptr->pos = pos;
			//ptr->pos += dir * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			//ptr->rot.z = acosf(dir.x) + RADIAN(45);
			ptr->pos.x += cosf(tmp) * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			ptr->pos.y += sinf(tmp) * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			ptr->rot.z = tmp + EFFECTEXPLOSION_BASEROTATION;

			//パラメータ設定
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->animTime = EFFECTEXPLOSION_INITPATTERN;
			//ptr->velocity = dir;
			ptr->velocity = D3DXVECTOR3(cosf(tmp), sinf(tmp), 0.0f);
			ptr->speed = ptr->initSpeed = EFFECTEXPLOSION_INITSPEED * RandomRange(EFFECTEXPLOSION_SPEEDMAGNI_MIN, EFFECTEXPLOSION_SPEEDMAGNI_MAX);
			ptr->scale = RandomRange(EFFECTEXPLOSION_SCALE_MIN, EFFECTEXPLOSION_SCALE_MAX);

			//セット
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}