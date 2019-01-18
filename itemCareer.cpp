//=============================================================================
//
// アイテムキャリア処理 [itemCareer.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "energyItem.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ITEMCAREER_SPEED			(7.0f)
#define ITEMCAREER_WAIT_FRAME		(240)
#define ITEMCAREER_ESCAPE_FRAME		(180)

#define ITEMCAREER_INITHP			(50)
#define ITEMCAREER_SCORE			(500)

#define ITEMCAREER_TARGETPOS_OFFSET	(D3DXVECTOR3(0.0f, 200.0f, 0.0f))
#define ITEMCAREER_SHADOW_OFFSETPOS	(D3DXVECTOR3(50.0f, 100.0f, 0.0f))
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexItemCareer(void);				// 頂点作成関数
void SetTextureItemCareer(int cntPattern);		// テクスチャ座標の計算処理
void SetVertexItemCareer(ITEMCAREER *ptr);		// 頂点の計算処理
void SetVertexItemCareerShadow(ITEMCAREER *ptr);

//各テクスチャ描画処理
void DrawItemCareerShadow(LPDIRECT3DDEVICE9 pDevice);
void DrawItemCareerBody(LPDIRECT3DDEVICE9 pDevice);

//更新処理
void UpdateItemCareerInit(ITEMCAREER *ptr);
void UpdateItemCareerWait(ITEMCAREER *ptr);
void UpdateItemCareerEscape(ITEMCAREER *ptr);

/*****************************************************************************
構造体定義
*****************************************************************************/
enum ITEMCAREER_TEX
{
	ITEMCAREER_SHADOW,
	ITEMCAREER_BODY,
	ITEMCAREER_TEXMAX
};

enum ITEMCAREER_STATE
{
	ITEMCAREER_INIT,
	ITEMCAREER_WAIT,
	ITEMCAREER_ESCAPE
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[ITEMCAREER_TEXMAX];	// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, radius;								//ポリゴン半径、角度

ITEMCAREER itemCareer[ITEMCAREER_MAX];					//アイテムキャリア配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitItemCareer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEMCAREER* ptr = GetItemCareerAdr(0);
	int i;

	angle = atan2f(ITEMCAREER_TEXTURE_SIZE_Y, ITEMCAREER_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ITEMCAREER_TEXTURE_SIZE_X, ITEMCAREER_TEXTURE_SIZE_Y));

	for (i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ITEMCAREER_INITPOS_X, ITEMCAREER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexItemCareer();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture[ITEMCAREER_SHADOW] = CreateTextureFromFile((LPSTR)ITEMCAREER_SHADOWTEX_NAME, pDevice);
		texture[ITEMCAREER_BODY] = CreateTextureFromFile((LPSTR)ITEMCAREER_TEXTURE_NAME, pDevice);
	}

	//SetItemCareer(D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f));

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitItemCareer(void)
{
	for (int i = 0; i < ITEMCAREER_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateItemCareer(void)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->cntFrame++;

		//撃墜処理
		if (ptr->hp <= 0)
		{
			SetEnergyItem(ptr->pos);
			AddScore(ITEMCAREER_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			ptr->active = false;
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % ITEMCAREER_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, ITEMCAREER_ANIM_NUM, ptr->patternAnim + 1);
		}

		//行動処理
		switch (ptr->state)
		{
		case ITEMCAREER_INIT:
			UpdateItemCareerInit(ptr);
			break;

		case ITEMCAREER_WAIT:
			UpdateItemCareerWait(ptr);
			break;

		case ITEMCAREER_ESCAPE:
			UpdateItemCareerEscape(ptr);
			break;
		}
	}
}

/******************************************************************************
更新処理(INIT状態)
******************************************************************************/
void UpdateItemCareerInit(ITEMCAREER *ptr)
{
	if (MoveToTarget(&ptr->pos, &ptr->targetPos, ITEMCAREER_SPEED))
	{
		ptr->cntFrame = 0;
		ptr->state = ITEMCAREER_WAIT;
	}
}

/******************************************************************************
更新処理(WAIT状態)
******************************************************************************/
void UpdateItemCareerWait(ITEMCAREER *ptr)
{
	if (ptr->cntFrame >= ITEMCAREER_WAIT_FRAME)
	{
		ptr->cntFrame = 0;
		ptr->state = ITEMCAREER_ESCAPE;
	}
}

/******************************************************************************
更新処理(ESCAPE状態)
******************************************************************************/
void UpdateItemCareerEscape(ITEMCAREER *ptr)
{
	StartAccel(&ptr->pos, &-VECTOR3_UP, ITEMCAREER_SPEED, ITEMCAREER_ESCAPE_FRAME, ptr->cntFrame);

	if (ptr->pos.y >= SCREEN_HEIGHT + ITEMCAREER_TEXTURE_SIZE_Y)
	{
		ptr->active = false;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawItemCareer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	DrawItemCareerShadow(pDevice);
	
	DrawItemCareerBody(pDevice);
}

/******************************************************************************
描画処理(影)
******************************************************************************/
void DrawItemCareerShadow(LPDIRECT3DDEVICE9 pDevice)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	pDevice->SetTexture(0, texture[ITEMCAREER_SHADOW]);
	
	SetTextureEnemyShadow(vertexWk);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		SetVertexItemCareerShadow(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
描画処理(本体)
******************************************************************************/
void DrawItemCareerBody(LPDIRECT3DDEVICE9 pDevice)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	pDevice->SetTexture(0, texture[ITEMCAREER_BODY]);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		SetVertexItemCareer(ptr);
		SetTextureItemCareer(ptr->patternAnim);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

#ifdef _DEBUG
		//当たり判定の描画
		DrawBoxCollider(ptr->pos + D3DXVECTOR3(ITEMCAREER_BB_LEFT, ITEMCAREER_BB_TOP, 0.0f), ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
#endif
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexItemCareer(void)
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
void SetTextureItemCareer(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % ITEMCAREER_TEXTURE_DIVIDE_X;
	int y = cntPattern / ITEMCAREER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ITEMCAREER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ITEMCAREER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexItemCareer(ITEMCAREER *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;
}

/******************************************************************************
頂点座標の設定(影)
******************************************************************************/
void SetVertexItemCareerShadow(ITEMCAREER *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;

	//影用にオフセット
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += ITEMCAREER_SHADOW_OFFSETPOS;
	}
}

/******************************************************************************
//関数名	：ITEMCAREER *GetItemCareerAdr(int pno)
//引数		：int pno：取得したいアイテムキャリアのインデックス
//戻り値	：正常な引数：アイテムキャリアのアドレス	異常な引数：NULL
//説明		：アイテムキャリアのアドレス取得関数
******************************************************************************/
ITEMCAREER *GetItemCareerAdr(int pno)
{
	if (pno < ITEMCAREER_MAX)
	{
		return &itemCareer[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetItemCareer(D3DXVECTOR3 pos)
//引数		：D3DXVECTOR3 pos		：出現座標
//戻り値	：void
//説明		：アイテムキャリアのセット関数
******************************************************************************/
void SetItemCareer(D3DXVECTOR3 pos)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->targetPos = ptr->pos + ITEMCAREER_TARGETPOS_OFFSET;

		ptr->state = ITEMCAREER_INIT;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->cntFrame = 0;
		ptr->hp = ITEMCAREER_INITHP;

		ptr->active = true;
		return;
	}
}