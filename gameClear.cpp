//=============================================================================
//
// ゲームオーバー処理 [gameClear.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "gameClear.h"
#include "transition.h"
#include "bgmPlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define GAMECLEAR_DIFFUSE_ALPHA			(3)
#define GAMECLEAR_FADEIN_FRAMEMAX		(85)
#define GAMECLEAR_ANIMATION_FRAMEMAX	(10)
#define GAMECLEAR_TRANSITION_FRAMEMAX	(180)
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexGameClear(void);				//頂点作成関数

void SetTextureGameClear(void);					// テクスチャ座標の計算処理
void SetTextureGameClearText(int i);				

void SetVertexGameClear(void);					// 頂点の計算処理
void SetVertexGameClearText(int i, GAMECLEAR *ptr);

void SetDiffuseGameClear(void);					//ディフューズの設定処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum GAMECLEAR_STATE
{
	GAMECLEAR_FADEIN,
	GAMECLEAR_ANIMATION,
	GAMECLEAR_TRANSITION,
	GAMECLEAR_STATEMAX
};

enum GAMECLEAR_TEXT
{
	GAMECLEAR_STAGE,
	GAMECLEAR_CLEAR,
	GAMECLEAR_TEXTMAX
};

enum GAMECLEAR_TEXTURE
{
	GAMECLEAR_BG,
	GAMECLEAR_TEXT,
	GAMECLEAR_TEXMAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[GAMECLEAR_TEXMAX];				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

GAMECLEAR gameClear;										//ゲームオーバー

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitGameClear(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAMECLEAR* ptr = &gameClear;

	ptr->pos = D3DXVECTOR3(GAMECLEAR_INITPOS_X, GAMECLEAR_INITPOS_Y, 0.0f);

	ptr->angle = atan2f(GAMECLEAR_TEXTURE_SIZE_Y, GAMECLEAR_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(GAMECLEAR_TEXTURE_SIZE_X, GAMECLEAR_TEXTURE_SIZE_Y));

	ptr->angleChar = atan2f(GAMECLEAR_CHARTEX_SIZE_Y, GAMECLEAR_CHARTEX_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(GAMECLEAR_CHARTEX_SIZE_X, GAMECLEAR_CHARTEX_SIZE_Y));

	ptr->active = false;
	ptr->state = GAMECLEAR_FADEIN;

	// 頂点情報の作成
	MakeVertexGameClear();
	SetTextureGameClear();
	SetVertexGameClear();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[GAMECLEAR_TEXT] = CreateTextureFromFile((LPSTR)GAMECLEAR_CHARTEX_NAME, pDevice);
		texture[GAMECLEAR_BG] = CreateTextureFromFile((LPSTR)GAMECLEAR_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitGameClear(void)
{
	for (int i = 0; i < GAMECLEAR_TEXMAX; i++)
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
void UpdateGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (!ptr->active)
	{
		return;
	}

	//フェードイン時の更新処理
	if (ptr->state == GAMECLEAR_FADEIN)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_FADEIN_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_FADEIN_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMECLEAR_ANIMATION;
		}
	}
	//文字をアニメーションさせる更新処理
	else if (ptr->state == GAMECLEAR_ANIMATION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_ANIMATION_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_ANIMATION_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMECLEAR_TRANSITION;
		}

	}
	//遷移待機時の更新処理
	else if (ptr->state == GAMECLEAR_TRANSITION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_TRANSITION_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_TRANSITION_FRAMEMAX)
		{
			FadeoutBGM(10);
			SetTransition(SCENE_NAMEENTRY);
			ptr->state = GAMECLEAR_STATEMAX;
		}
	}


}

/******************************************************************************
描画処理
******************************************************************************/
void DrawGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (!ptr->active)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, texture[GAMECLEAR_BG]);

	//頂点を設定
	SetDiffuseGameClear();
	SetTextureGameClear();
	SetVertexGameClear();

	//テクスチャブレンディングを設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	if (ptr->state == GAMECLEAR_FADEIN)
	{
		return;
	}

	//テクスチャブレンディングを再設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//テキストを描画
	pDevice->SetTexture(0, texture[GAMECLEAR_TEXT]);
	for (int i = 0; i < GAMECLEAR_TEXTMAX; i++)
	{
		SetTextureGameClearText(i);
		SetVertexGameClearText(i, ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexGameClear(void)
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
void SetTextureGameClear(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定(テキスト用)
******************************************************************************/
void SetTextureGameClearText(int pattern)
{
	float sizeY = 1.0f / GAMECLEAR_CHATEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, pattern * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, pattern * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, pattern * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, pattern * sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
頂点座標の設定(テキスト用)
******************************************************************************/
void SetVertexGameClearText(int pattern, GAMECLEAR *ptr)
{
	D3DXVECTOR3 pos;

	switch (pattern)
	{
	case GAMECLEAR_STAGE:
		pos = D3DXVECTOR3(GAMECLEAR_STAGETEX_INITPOS_X, GAMECLEAR_STAGETEX_INITPOS_Y, 0.0f);
		break;

	case GAMECLEAR_CLEAR:
		pos = D3DXVECTOR3(GAMECLEAR_CLEARTEX_INITPOS_X, GAMECLEAR_CLEARTEX_INITPOS_Y, 0.0f);
		break;

	default:
		pos = D3DXVECTOR3(GAMECLEAR_CLEARTEX_INITPOS_X, GAMECLEAR_CLEARTEX_INITPOS_Y, 0.0f);
		break;
	}

	if (ptr->state == GAMECLEAR_ANIMATION)
	{
		pos.x += (SCREEN_CENTER_X - pos.x) / GAMECLEAR_ANIMATION_FRAMEMAX * ptr->cntFrame;

		vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	}
	else
	{
		pos.x = SCREEN_CENTER_X;

		vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	}
}

/******************************************************************************
ディフューズの設定
******************************************************************************/
void SetDiffuseGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (ptr->state != GAMECLEAR_FADEIN)
	{
		return;
	}

	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, ptr->cntFrame * GAMECLEAR_DIFFUSE_ALPHA);
}

/******************************************************************************
//関数名	：GAMECLEAR *GetGameClearAdr(int pno)
//引数		：int pno：取得したいゲームオーバーのインデックス
//戻り値	：正常な引数：ゲームオーバーのアドレス	異常な引数：NULL
//説明		：ゲームオーバーのアドレス取得関数
******************************************************************************/
GAMECLEAR *GetGameClearAdr(void)
{

	return &gameClear;
}

/******************************************************************************
//関数名	：void SetGameClear(void)
//引数		：void
//戻り値	：void
//説明		：ゲームオーバーのset関数
******************************************************************************/
void SetGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	ptr->active = true;
	ptr->state = GAMECLEAR_FADEIN;
	ptr->cntFrame = 0;
	PlayBGM(BGM_CLEAR);
	SetScene(SCENE_CLEAR);
}