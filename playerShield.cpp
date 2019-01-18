//=============================================================================
//
// プレイヤーバレット処理 [playerShield.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "playerShield.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYERSHIELD_SPEED				(-40.0f)

#define PLAYERSHIELD_SCALE_MAX			(1.0f)
#define PLAYERSHIELD_SCALE_MIN			(0.0f)
#define PLAYERSHIELD_SCALE_ADDVALUE		(0.1f)

#define PLAYERSHIELD_COLOR_GREEN		(D3DCOLOR_RGBA(204, 255, 0, 128))
#define PLAYERSHIELD_COLOR_ORANGE		(D3DCOLOR_RGBA(255, 102, 0, 128))

#define PLAYERSHIELD_POS_OFFSET			(D3DXVECTOR3(0.0f, -10.0f, 0.0f))

#define PLAYERSHIELD_OPEN_FRAME			(180)
#define PLAYERSHIELD_CLOSE_FRAME		(120)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerShield(void);					//頂点作成関数
void SetTexturePlayerShield(int cntPattern);			// テクスチャ座標の計算処理
void SetVertexPlayerShield(PLAYERSHIELD *ptr);			// 頂点の計算処理
void SetDiffusePlayerShield(PLAYERSHIELD *ptr);			//ディフューズ設定処理
void SetPlayerShieldColor(PLAYERSHIELD *ptr);			//状態設定
void SetPlayerShieldAlpha(int val);

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

PLAYERSHIELD playerShield;								//プレイヤーバレット配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerShield(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERSHIELD* ptr = GetPlayerShieldAdr();

	ptr->pos = D3DXVECTOR3(PLAYERSHIELD_INITPOS_X, PLAYERSHIELD_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(PLAYERSHIELD_TEXTURE_SIZE_Y, PLAYERSHIELD_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERSHIELD_TEXTURE_SIZE_X, PLAYERSHIELD_TEXTURE_SIZE_Y));
	ptr->scale = 1.0f;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;
	ptr->cntFrame = 0;

	//ptr->active = false;

	// 頂点情報の作成
	MakeVertexPlayerShield();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)PLAYERSHIELD_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerShield(void)
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
void UpdatePlayerShield(void)
{
	PLAYERSHIELD *ptr = &playerShield;
	PLAYER *player = GetPlayerAdr(0);

	if (ptr->active)
	{
		//if (player->state == PLAYER_INVINCIBLE)
		//{
			ptr->cntFrame++;
		//}

		if (ptr->cntFrame > PLAYERSHIELD_OPEN_FRAME + PLAYERSHIELD_CLOSE_FRAME)
		{
			ptr->active = false;
			return;
		}

		if (ptr->cntFrame > PLAYERSHIELD_OPEN_FRAME)
		{
			float percent = (PLAYERSHIELD_OPEN_FRAME + PLAYERSHIELD_CLOSE_FRAME - ptr->cntFrame) / (float)PLAYERSHIELD_CLOSE_FRAME;
			int alpha = (int)(percent * 255);
			SetPlayerShieldAlpha(alpha);
		}

		ptr->pos = player->pos + PLAYERSHIELD_POS_OFFSET;

		ptr->active = (ptr->scale != PLAYERSHIELD_SCALE_MIN) ? true : false;


	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerShield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERSHIELD *ptr = GetPlayerShieldAdr();

	if (!ptr->active)
	{
		return;
	}

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//頂点座標を設定
	SetVertexPlayerShield(ptr);

	// テクスチャ座標を設定
	SetTexturePlayerShield(ptr->patternAnim);

	//ディフューズ設定(色変更はオミット)
	//SetDiffusePlayerShield(ptr);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerShield(void)
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
void SetTexturePlayerShield(int cntPattern)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerShield(PLAYERSHIELD *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

void SetDiffusePlayerShield(PLAYERSHIELD *ptr)
{
	/*D3DCOLOR color = (ptr->color == SHIELD_GREEN) ? PLAYERSHIELD_COLOR_GREEN : PLAYERSHIELD_COLOR_ORANGE;

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].diffuse = color;
	}*/
}

/******************************************************************************
//関数名	：PLAYERSHIELD *GetPlayerShieldAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数
******************************************************************************/
PLAYERSHIELD *GetPlayerShieldAdr(void)
{
	return &playerShield;
}

void SetPlayerShield(void)
{
	PLAYERSHIELD *ptr = &playerShield;

	if (!ptr->active)
	{
		SetPlayerShieldAlpha(255);
		ptr->cntFrame = 0;
		ptr->active = true;
	}
}

void SetPlayerShieldColor(PLAYERSHIELD *ptr)
{
	if (GetMyButton1Down(0))
	{
		ptr->active = true;
		//ptr->color = SHIELD_ORANGE;
		return;
	}

	if (GetMyButton2Down(0))
	{
		ptr->active = true;
		//ptr->color = SHIELD_GREEN;
	}

	ptr->active = false;
}

void SetPlayerShieldAlpha(int val)
{
	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, val);
}