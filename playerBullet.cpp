//=============================================================================
//
// プレイヤーバレット処理 [playerBullet.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "playerBullet.h"
#include "bg.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYERBULLET_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPlayerBullet(PLAYERBULLET *ptr);				//頂点作成関数
void SetTexturePlayerBullet(int cntPattern, PLAYERBULLET *ptr);	// テクスチャ座標の計算処理
void SetVertexPlayerBullet(PLAYERBULLET *ptr);					// 頂点の計算処理

/*****************************************************************************
グローバル変数
*****************************************************************************/
LPDIRECT3DTEXTURE9		textureMainBullet = NULL;				// テクスチャへのポインタ（メイン用）
LPDIRECT3DTEXTURE9		textureSubBullet = NULL;				// テクスチャへのポインタ（サブ用）

PLAYERBULLET playerBullet[PLAYERBULLET_MAIN_MAX];				//プレイヤーバレット配列(メインショット)
PLAYERBULLET playerSubBullet[PLAYERBULLET_SUB_MAX];				//プレイヤーバレット配列（サブショット）

static VERTEX_2D		vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

#ifdef _DEBUG
static int cntBullet;
#endif

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayerBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBULLET* ptr = GetPlayerBulletAdr(0);
	int i;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBULLET_INITPOS_X, PLAYERBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBULLET_TEXTURE_SIZE_Y, PLAYERBULLET_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBULLET_TEXTURE_SIZE_X, PLAYERBULLET_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->moveDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		ptr->active = false;

		// 頂点情報の作成
		MakeVertexPlayerBullet(ptr);
	}

	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBULLET_INITPOS_X, PLAYERBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBULLET_TEXTURE_SIZE_Y, PLAYERBULLET_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBULLET_TEXTURE_SIZE_X, PLAYERBULLET_TEXTURE_SIZE_Y));
	
		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->moveDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		ptr->active = false;

		// 頂点情報の作成
		MakeVertexPlayerBullet(ptr);
	}

	if (num == 0)
	{
		// テクスチャの読み込み
		textureMainBullet = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXTURE_MAIN, pDevice);
		textureSubBullet = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXTURE_SUB, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayerBullet(void)
{
	if (textureMainBullet != NULL)
	{	// テクスチャの開放
		textureMainBullet->Release();
		textureMainBullet = NULL;
	}

	if (textureSubBullet != NULL)
	{
		textureSubBullet->Release();
		textureMainBullet = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdatePlayerBullet(void)
{
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	int i;

	//メインバレットの更新
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//座標の更新
			ptr->pos += PLAYERBULLET_SPEED * ptr->moveDir;
			if (ptr->pos.y < -PLAYERBULLET_BB_WIDTH|| ptr->pos.y > SCREEN_HEIGHT + PLAYERBULLET_BB_HIGHT)
			{
				ptr->active = false;
			}
			else if (ptr->pos.x + PLAYERBULLET_BB_WIDTH < BG_LEFT || ptr->pos.x - PLAYERBULLET_BB_WIDTH > BG_RIGHT)
			{
				ptr->active = false;
			}

			// アニメーション
			ptr->countAnim++;

			// アニメーションWaitチェック
			if (ptr->countAnim % TIMEPLAYERBULLET_ANIMATION == 0)
			{
				// パターンの切り替え
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PLAYERBULLET_NUM;
			}
		}
	}

	//サブバレットの更新
	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//座標の更新
			ptr->pos += PLAYERBULLET_SPEED * ptr->moveDir;
			if (ptr->pos.y < -PLAYERBULLET_BB_HIGHT || ptr->pos.y > SCREEN_HEIGHT + PLAYERBULLET_BB_HIGHT)
			{
				ptr->active = false;
			}
			else if (ptr->pos.x + PLAYERBULLET_BB_WIDTH < BG_LEFT || ptr->pos.x - PLAYERBULLET_BB_WIDTH > BG_RIGHT)
			{
				ptr->active = false;
			}

			// アニメーション
			ptr->countAnim++;

			// アニメーションWaitチェック
			if (ptr->countAnim % TIMEPLAYERBULLET_ANIMATION == 0)
			{
				// パターンの切り替え
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PLAYERBULLET_NUM;
			}
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, textureMainBullet);

	//プレイヤーバレットを加算合成で描画
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPlayerBullet(ptr);

			// テクスチャ座標を設定
			SetTexturePlayerBullet(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBULLET_BB_LEFT, PLAYERBULLET_BB_TOP, 0.0f), PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
#endif
		}
	}

	// テクスチャの設定
	pDevice->SetTexture(0, textureSubBullet);

	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexPlayerBullet(ptr);

			// テクスチャ座標を設定
			SetTexturePlayerBullet(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBULLET_BB_LEFT, PLAYERBULLET_BB_TOP, 0.0f), PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
#endif
		}
	}

	//描画方法を通常の方法に再設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPlayerBullet(PLAYERBULLET *ptr)
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
void SetTexturePlayerBullet(int cntPattern, PLAYERBULLET *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % PLAYERBULLET_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYERBULLET_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYERBULLET_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYERBULLET_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPlayerBullet(PLAYERBULLET *ptr)
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
//関数名	：PLAYERBULLET *GetPlayerBulletAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（メインショット）
******************************************************************************/
PLAYERBULLET *GetPlayerBulletAdr(int pno)
{
	if (pno < PLAYERBULLET_MAIN_MAX)
	{
		return &playerBullet[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：PLAYERBULLET *GetPlayerSubBulletAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（サブショット）
******************************************************************************/
PLAYERBULLET *GetPlayerSubBulletAdr(int pno)
{
	if (pno < PLAYERBULLET_SUB_MAX)
	{
		return &playerSubBullet[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void FirePlayerBullet(D3DXVECTOR3 pos)
//引数		：D3DXVECTOR3 pos：プレイヤーバレットの発射位置
//戻り値	：void
//説明		：プレイヤーバレットを発射する関数（メインショット用）
******************************************************************************/
void FirePlayerBullet(D3DXVECTOR3 pos)
{
	int i;
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		//非アクティブのプレイヤーバレットがあれば発射
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->active = true;
			return;
		}
	}
}
/******************************************************************************
//関数名	：void FirePlayerSubBullet(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos：プレイヤーバレットの発射位置
//引数2		：float rotAngle：バレットを回転させる角度
//戻り値	：void
//説明		：プレイヤーバレットを発射する関数（サブショット用）
******************************************************************************/
void FirePlayerSubBullet(D3DXVECTOR3 pos, float rotAngle)
{
	int i;
	PLAYERBULLET *ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		//非アクティブのプレイヤーバレットがあれば発射
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->moveDir = D3DXVECTOR3(cosf(RADIAN(rotAngle)), sinf(RADIAN(rotAngle)), 0.0f);
			ptr->rot.z = RADIAN(90 + rotAngle);
			ptr->active = true;
			return;
		}
	}
}
