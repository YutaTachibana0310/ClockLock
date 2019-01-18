//=============================================================================
//
// プレイヤーゲージ処理 [tmplate.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "playerGauge.h"
#include "input.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYERGAUGE_TEXTURE_CENTER_X	(0.5f)									//テクスチャ座標の中心（X)
#define PLAYERGAUGE_TEXTURE_CENTER_Y	(0.5f)									//テクスチャ座標の中心（Y)
#define PLAYERGAUGE_INITANGLE			(90)									//ゲージの初期角度
#define PLAYERGAUGE_PERCENT_DEVIDENUM	(720 / (PLAYERGAUGE_VERTEX_NUM - 2))	//プレイヤーゲージのポリゴン数を求めるのに使用する値
#define PLAYERGAUGE_OFFSETPOS			(D3DXVECTOR3(0.0f, -10.0f, 0.0f))		//プレイヤーの位置からゲージをずらして表示する座標

#define PLAYERGAUGE_SCALE_MAX			(1.5f)				//プレイヤーゲージスケール最大値
#define PLAYERGAUGE_ADDSCALE			(0.2f)				//プレイヤーゲージスケール変化値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerGauge(void);							//頂点作成関数
void SetTexturePlayerGauge(PLAYERGAUGE *ptr);					//テクスチャ座標の計算処理
void SetTexturePlayerGaugeBack(PLAYERGAUGE *ptr);				//テクスチャ座標の計算処理（バック用）
void SetVertexPlayerGauge(PLAYERGAUGE *ptr);					//頂点の計算処理
void SetVertexPlayerGaugeBack(PLAYERGAUGE *ptr);				//頂点の計算処理（バック用）

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;							// テクスチャへのポインタ（メイン用）
static LPDIRECT3DTEXTURE9		texBack = NULL;							// バック用テクスチャ

PLAYERGAUGE						playerGauge[PLAYERGAUGE_MAX];			//プレイヤーゲージ配列

static VERTEX_2D				vertexWk[PLAYERGAUGE_VERTEX_NUM];		//頂点情報格納ワーク
static VERTEX_2D				backVertexWk[NUM_VERTEX];				//バック用頂点情報格納ワーク
static D3DXVECTOR2				vertexOffset[PLAYERGAUGE_VERTEX_NUM];	//拡張点の中心からのオフセット座標					

static int par = 0;

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERGAUGE* ptr = GetPlayerGaugeAdr(0);
	int i;

	for (i = 0; i < PLAYERGAUGE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERGAUGE_INITPOS_X, PLAYERGAUGE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERGAUGE_TEXTURE_SIZE_Y, PLAYERGAUGE_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERGAUGE_TEXTURE_SIZE_X, PLAYERGAUGE_TEXTURE_SIZE_Y));

		ptr->percent = PLAYER_TIMEEFFECT_MAX;
		ptr->scale = 0.0f;
		ptr->active = true;

		ptr->state = GAUGESTATE_NORMAL;

		// 頂点情報の作成
		MakeVertexPlayerGauge();
	}

	//頂点のオフセット座標の計算
	float angle = 360.0f / (PLAYERGAUGE_VERTEX_NUM - 2);
	vertexOffset[0] = D3DXVECTOR2(0.0f, 0.0f);
	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexOffset[i] = D3DXVECTOR2(cosf(RADIAN(-90 + angle * (i - 1))), sinf(RADIAN(-90 + angle * (i - 1))));
	}

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PLAYERGAUGE_TEXTURE_NAME, pDevice);
		texBack = CreateTextureFromFile((LPSTR)PLAYERGAUGE_BACKTEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerGauge(void)
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
void UpdatePlayerGauge(void)
{
	PLAYERGAUGE *ptr = GetPlayerGaugeAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	ptr->pos = player->pos + PLAYERGAUGE_OFFSETPOS;
	ptr->rot.z += 0.005f;

	//ゲージの状態がOPEN or CLOSE状態であればscaleを変化させる
	if (ptr->state == GAUGESTATE_OPEN)
	{
		ptr->scale = Clampf(0.0f, PLAYERGAUGE_SCALE_MAX, ptr->scale + PLAYERGAUGE_ADDSCALE);
	}

	else if (ptr->state == GAUGESTATE_CLOSE)
	{
		ptr->scale = Clampf(0.0f, PLAYERGAUGE_SCALE_MAX, ptr->scale - PLAYERGAUGE_ADDSCALE);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERGAUGE *ptr = GetPlayerGaugeAdr(0);
	int i, numPolygon;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//プレイヤーゲージを描画
	for (i = 0; i < PLAYERGAUGE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			// ゲージ背景描画
			pDevice->SetTexture(0, texBack);
			SetVertexPlayerGaugeBack(ptr);
			SetTexturePlayerGaugeBack(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, backVertexWk, sizeof(VERTEX_2D));

			// ゲージ描画
			pDevice->SetTexture(0, texture);
			SetVertexPlayerGauge(ptr);
			SetTexturePlayerGauge(ptr);
			numPolygon = (int)ptr->percent / PLAYERGAUGE_PERCENT_DEVIDENUM;
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numPolygon, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	for (i = 0; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		// rhwの設定
		vertexWk[i].rhw = 1.0f;
		// 反射光の設定
		vertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	for (i = 0; i < NUM_VERTEX; i++)
	{
		// rhwの設定
		backVertexWk[i].rhw = 1.0f;
		// 反射光の設定
		backVertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	}

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTexturePlayerGauge(PLAYERGAUGE *ptr)
{
	int i;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(PLAYERGAUGE_TEXTURE_CENTER_X, PLAYERGAUGE_TEXTURE_CENTER_Y);

	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexWk[i].tex.x = 0.5f * (1 + vertexOffset[i].x);
		vertexWk[i].tex.y = 0.5f * (1 + vertexOffset[i].y);
	}
}

void SetTexturePlayerGaugeBack(PLAYERGAUGE *ptr)
{
	//テクスチャ座標の設定
	backVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	backVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	backVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	backVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerGauge(PLAYERGAUGE *ptr)
{
	int i;

	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x;
	vertexWk[0].vtx.y = ptr->pos.y;
	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexWk[i].vtx.x = ptr->pos.x + vertexOffset[i].x * PLAYERGAUGE_TEXTURE_SIZE_X * ptr->scale;
		vertexWk[i].vtx.y = ptr->pos.y + vertexOffset[i].y * PLAYERGAUGE_TEXTURE_SIZE_Y * ptr->scale;
	}
}

void SetVertexPlayerGaugeBack(PLAYERGAUGE *ptr)
{
	backVertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//関数名	：PLAYERGAUGE *GetPlayerGaugeAdr(int pno)
//引数		：int pno：取得したいプレイヤーゲージのインデックス
//戻り値	：正常な引数：プレイヤーゲージのアドレス	異常な引数：NULL
//説明		：プレイヤーゲージのアドレス取得関数
******************************************************************************/
PLAYERGAUGE *GetPlayerGaugeAdr(int pno)
{
	if (pno < PLAYERGAUGE_MAX)
	{
		return &playerGauge[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetGaugeState(GAUGESTATE state)
//引数1		：GAUGESTATE state	：セットしたいゲージの状態
//引数2		：int no			：セットしたいゲージのインデックス
//戻り値	：void
//説明		：プレイヤーゲージの状態セット関数
******************************************************************************/
void SetGaugeState(GAUGESTATE state, int no)
{
	if (no < PLAYERGAUGE_MAX)
	{
		PLAYERGAUGE *ptr = GetPlayerGaugeAdr(no);

		ptr->state = state;
		if (state == GAUGESTATE_OPEN)
		{
			ptr->active = true;
		}
	}
}