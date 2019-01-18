//=============================================================================
//
// エフェクトファイア処理 [effectFire.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "effectFire.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define EFFECTFIRE_INITPOS_RANGE_MAX			(50.0f)			//エフェクトファイアの座標設定範囲
#define EFFECTFIRE_INITPOS_RANGE_MIN			(0.0f)			//エフェクトファイアの座標設定範囲
#define EFFECTFIRE_INITSPEED					(3.0f)			//エフェクトファイアの初期速度
#define EFFECTFIRE_ROTATION_MIN					(0.0f)			//回転量最小値
#define EFFECTFIRE_ROTATION_MAX					(360.0f)		//回転量最大値
#define EFFECTFIRE_SPEEDMAGNI_MIN				(0.8f)			//スピード倍率最小値
#define EFFECTFIRE_SPEEDMAGNI_MAX				(1.2f)			//スピード倍率最大値
#define EFFECTFIRE_SCALE_MAX					(0.5f)			//スケール最小値
#define EFFECTFIRE_SCALE_MIN					(1.0f)			//スケール最大値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEffectFire(void);						//頂点作成関数
void SetTextureEffectFire(int cntPattern, EFFECT *ptr);	// テクスチャ座標の計算処理
void SetVertexEffectFire(EFFECT *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float radius, angle;								//ポリゴンの半径と頂点角度

EFFECT effectFire[EFFECTFIRE_MAX];						//エフェクトファイア配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEffectFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectFireAdr(0);
	int i;

	angle = atan2f(EFFECTFIRE_TEXTURE_SIZE_Y, EFFECTFIRE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTFIRE_TEXTURE_SIZE_X, EFFECTFIRE_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTFIRE_INITPOS_X, EFFECTFIRE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->animTime = EFFECTFIRE_ANIM_TIME;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEffectFire();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)EFFECTFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEffectFire(void)
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
void UpdateEffectFire(void)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
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
			if (ptr->patternAnim >= EFFECTFIRE_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//座標の更新
		if (ptr->countAnim <= 12)
		{
			ptr->speed = cosf(RADIAN(7.5f * ptr->countAnim)) * ptr->initSpeed;
		}
		ptr->pos += ptr->velocity  * ptr->speed;

	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEffectFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectFireAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エフェクトファイアを描画
	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEffectFire(ptr);

			// テクスチャ座標を設定
			SetTextureEffectFire(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEffectFire(void)
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
void SetTextureEffectFire(int cntPattern, EFFECT *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % EFFECTFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEffectFire(EFFECT *ptr)
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
//関数名	：EFFECTFIRE *GetEffectFireAdr(int pno)
//引数		：int pno：取得したいエフェクトファイアのインデックス
//戻り値	：正常な引数：エフェクトファイアのアドレス	異常な引数：NULL
//説明		：エフェクトファイアのアドレス取得関数
******************************************************************************/
EFFECT *GetEffectFireAdr(int pno)
{
	if (pno < EFFECTFIRE_MAX)
	{
		return &effectFire[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：EFFECT *SetEffectFire(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos		：設定座標
//引数2		：D3DXVECTOR3 dir		：進む方向
//戻り値	：成功時：セットしたエフェクトへのポインタ	失敗時：NULL
//説明		：エフェクトファイアのセット関数
******************************************************************************/
EFFECT *SetEffectFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		//未使用のエフェクトファイアがあればセット
		if (!ptr->active)
		{
			//座標の設定
			ptr->pos = pos;
			ptr->pos += dir * RandomRange(EFFECTFIRE_INITPOS_RANGE_MIN, EFFECTFIRE_INITPOS_RANGE_MAX);

			//パラメータ設定
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->velocity = dir;
			ptr->rot.z = RandomRange(EFFECTFIRE_ROTATION_MIN, EFFECTFIRE_ROTATION_MAX);
			ptr->scale = RandomRange(0.5f, 1.0f);
			ptr->speed = ptr->initSpeed = EFFECTFIRE_INITSPEED * RandomRange(EFFECTFIRE_SPEEDMAGNI_MIN, EFFECTFIRE_SPEEDMAGNI_MAX);

			//セット
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}

/******************************************************************************
//関数名	：EFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed)
//引数1		：D3DXVECTOR3 pos		：設定座標
//引数2		：D3DXVECTOR3 dir		：進む方向
//引数3		：float speed			：スピード
//戻り値	：成功時：セットしたエフェクトへのポインタ	失敗時：NULL
//説明		：ボスファイアのセット関数
******************************************************************************/
EFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	//未使用のエフェクトファイアがあればセット
	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		//座標の設定
		ptr->pos = pos;
		
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->velocity = dir;
		ptr->rot.z = RandomRange(EFFECTFIRE_ROTATION_MIN, EFFECTFIRE_ROTATION_MAX);
		ptr->scale = RandomRange(EFFECTFIRE_SCALE_MIN, EFFECTFIRE_SCALE_MAX);
		ptr->speed = ptr->initSpeed = speed;

		//セット
		ptr->active = true;
		return ptr;
	}

	return NULL;
}