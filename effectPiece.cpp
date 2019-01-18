//=============================================================================
//
// エフェクトピース処理 [effectPiece.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "effectPiece.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define EFFECTPIECE_POS_RANGE				(0.0f)			//エフェクトピースの座標設定範囲
#define EFFECTPIECE_INITSPEED				(10.0f)			//エフェクトピースの初期速度
#define EFFECTPIECE_SLOWFRAME				(16)			//減速するフレーム
#define EFFECTPIECE_SCALE_MIN				(0.3f)			//スケール最小値
#define EFFECTPIECE_SCALE_MAX				(2.5f)			//スケール最大値
#define EFFECTPIECE_SPEED_MIN				(0.3f)			//スピード最小値	
#define EFFECTPIECE_SPEED_MAX				(2.5f)			//スピード最大値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEffectPiece(void);						//頂点作成関数
void SetTextureEffectPiece(int cntPattern, EFFECT *ptr);	// テクスチャ座標の計算処理
void SetVertexEffectPiece(EFFECT *ptr);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float radius, angle;

EFFECT effectPiece[EFFECTPIECE_MAX];					//エフェクトピース配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEffectPiece(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectPieceAdr(0);
	int i;

	angle = atan2f(EFFECTPIECE_TEXTURE_SIZE_Y, EFFECTPIECE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTPIECE_TEXTURE_SIZE_X, EFFECTPIECE_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTPIECE_INITPOS_X, EFFECTPIECE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->animTime = EFFECTPIECE_ANIM_TIME;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEffectPiece();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)EFFECTPIECE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEffectPiece(void)
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
void UpdateEffectPiece(void)
{
	int i;
	EFFECT *ptr = GetEffectPieceAdr(0);

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
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
			if (ptr->patternAnim >= EFFECTPIECE_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//座標の更新
		int animMaxHalf = EFFECTPIECE_ANIM_NUM * ptr->animTime;
		if (ptr->countAnim <= 14)
		{
			ptr->speed = cosf(RADIAN(ptr->countAnim * 90.0f / animMaxHalf)) * ptr->initSpeed;
		}
		ptr->pos += ptr->velocity * ptr->speed;

	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEffectPiece(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectPieceAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//エフェクトピースを描画
	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEffectPiece(ptr);

			// テクスチャ座標を設定
			SetTextureEffectPiece(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEffectPiece(void)
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
void SetTextureEffectPiece(int cntPattern, EFFECT *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % EFFECTPIECE_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTPIECE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTPIECE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTPIECE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEffectPiece(EFFECT *ptr)
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
//関数名	：EFFECTPIECE *GetEffectPieceAdr(int pno)
//引数		：int pno：取得したいエフェクトピースのインデックス
//戻り値	：正常な引数：エフェクトピースのアドレス	異常な引数：NULL
//説明		：エフェクトピースのアドレス取得関数
******************************************************************************/
EFFECT *GetEffectPieceAdr(int pno)
{
	if (pno < EFFECTPIECE_MAX)
	{
		return &effectPiece[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：EFFECT *SetEffectPiece(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos		：設定座標
//引数2		：D3DXVECTOR3 dir		：進む方向
//戻り値	：成功時：セットしたエフェクトへのポインタ	失敗時：NULL
//説明		：エフェクトピースのセット関数
******************************************************************************/
EFFECT *SetEffectPiece(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectPieceAdr(0);

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		//未使用のエフェクトピースがあればセット
		if (!ptr->active)
		{
			//座標の設定
			ptr->pos = pos;
			ptr->pos.x += RandomRange(-EFFECTPIECE_POS_RANGE, EFFECTPIECE_POS_RANGE);
			ptr->pos.y += RandomRange(-EFFECTPIECE_POS_RANGE, EFFECTPIECE_POS_RANGE);
			ptr->rot.z = RandomRange(0.0f, 360.0f);

			//パラメータ設定
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->animTime = EFFECTPIECE_ANIM_TIME;
			ptr->velocity = dir;
			ptr->speed = ptr->initSpeed = EFFECTPIECE_INITSPEED * RandomRange(EFFECTPIECE_SPEED_MIN, EFFECTPIECE_SPEED_MAX);
			ptr->scale = RandomRange(EFFECTPIECE_SCALE_MIN, EFFECTPIECE_SCALE_MAX);

			//セット
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}