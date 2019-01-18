//=============================================================================
//
// エネミーゲージ処理 [enemyGauge.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyGauge.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ENEMYGAUGE_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnemyGauge(void);									//頂点作成関数
void SetTextureEnemyGauge(int cntPattern, ENEMYGAUGE *ptr);			//テクスチャ座標の計算処理
void SetTextureEnemyGaugeFrame(int cntPattern, ENEMYGAUGE *ptr);	//テクスチャ座標の計算処理（フレーム用）
void SetVertexEnemyGauge(ENEMYGAUGE *ptr);							//頂点の計算処理
void SetVertexEnemyGaugeFrame(ENEMYGAUGE *ptr);						//頂点の計算処理（フレーム用）
void SetDiffuseEnemyGauge(ENEMYGAUGE *ptr);							//頂点色の計算処理
void ResetDiffuseEnemyGauge();										//頂点色のリセット処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;				// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		textureFrame = NULL;		// テクスチャへのポインタ（フレーム用)
static LPDIRECT3DTEXTURE9		textureBG = NULL;			// テクスチャへのポインタ（背景用)

ENEMYGAUGE enemyGauge[ENEMYGAUGE_MAX];						//プレイヤーバレット配列

static VERTEX_2D		vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnemyGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYGAUGE* ptr = GetEnemyGaugeAdr(0);
	int i;

	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYGAUGE_INITPOS_X, ENEMYGAUGE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexEnemyGauge();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)ENEMYGAUGE_TEXTURE_NAME, pDevice);
		textureFrame = CreateTextureFromFile((LPSTR)ENEMYGAUGEFRAME_TEXTURE_NAME, pDevice);
		textureBG = CreateTextureFromFile((LPSTR)ENEMYGAUGEBG_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnemyGauge(void)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}

	if (textureFrame != NULL)
	{
		textureFrame->Release();
		textureFrame = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateEnemyGauge(void)
{
	//ゲージの座標更新、スケール更新は親（エネミー）の更新処理内で行う
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnemyGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYGAUGE *ptr = GetEnemyGaugeAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲージを描画
	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//ゲージ背景を描画
			pDevice->SetTexture(0, textureBG);
			ResetDiffuseEnemyGauge();
			SetVertexEnemyGaugeFrame(ptr);
			SetTextureEnemyGaugeFrame(ptr->patternAnim, ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

			//ゲージを描画
			pDevice->SetTexture(0, texture);
			SetVertexEnemyGauge(ptr);
			SetTextureEnemyGauge(ptr->patternAnim, ptr);
			SetDiffuseEnemyGauge(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

			//ゲージフレームを描画
			pDevice->SetTexture(0, textureFrame);
			ResetDiffuseEnemyGauge();
			SetVertexEnemyGaugeFrame(ptr);
			SetTextureEnemyGaugeFrame(ptr->patternAnim, ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnemyGauge(void)
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
テクスチャ座標の設定(ゲージ用)
******************************************************************************/
void SetTextureEnemyGauge(int cntPattern, ENEMYGAUGE *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % ENEMYGAUGE_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->scale, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->scale, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定(ゲージフレーム用)
******************************************************************************/
void SetTextureEnemyGaugeFrame(int cntPattern, ENEMYGAUGE *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % ENEMYGAUGE_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定(ゲージ用)
******************************************************************************/
void SetVertexEnemyGauge(ENEMYGAUGE *ptr)
{
	float gaugePosX = ptr->pos.x - ENEMYGAUGE_TEXTURE_SIZE_X / 2;
	float gaugePosY = ptr->pos.y - ENEMYGAUGE_TEXTURE_SIZE_Y / 2;

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(gaugePosX, gaugePosY, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X * ptr->scale, gaugePosY, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(gaugePosX, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X * ptr->scale, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
頂点座標の設定(ゲージフレーム用)
******************************************************************************/
void SetVertexEnemyGaugeFrame(ENEMYGAUGE *ptr)
{
	float gaugePosX = ptr->pos.x - ENEMYGAUGE_TEXTURE_SIZE_X / 2;
	float gaugePosY = ptr->pos.y - ENEMYGAUGE_TEXTURE_SIZE_Y / 2;

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(gaugePosX, gaugePosY, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X, gaugePosY, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(gaugePosX, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
頂点色の設定
******************************************************************************/
void SetDiffuseEnemyGauge(ENEMYGAUGE *ptr)
{
	//緑から黄色への遷移
	if (ptr->scale > 0.5f)
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA((int)((1.0f - ptr->scale) * 510), 255, 0, 255);
	}
	//黄色から赤への遷移
	else if (ptr->scale > 0.1f)
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA(255, (int)((ptr->scale * 2.5f - 0.25f) * 255), 0, 255);
	}
	//残り少ない時は赤のみ
	else
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
}

/******************************************************************************
頂点色のリセット
******************************************************************************/
void ResetDiffuseEnemyGauge()
{
	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}

/******************************************************************************
//関数名	：ENEMYGAUGE *GetEnemyGaugeAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（メインショット）
******************************************************************************/
ENEMYGAUGE *GetEnemyGaugeAdr(int pno)
{
	if (pno < ENEMYGAUGE_MAX)
	{
		return &enemyGauge[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：ENEMYGAUGE *GetNonActiveEnemyGauge(void)
//引数		：void
//戻り値	：非アクティブのエネミーゲージへのポインタ
//説明		：非アクティブのエネミーゲージを取得する関数
******************************************************************************/
ENEMYGAUGE *GetNonActiveEnemyGauge(void)
{
	int i;
	ENEMYGAUGE *ptr = GetEnemyGaugeAdr(0);

	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->scale = 1.0f;
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}