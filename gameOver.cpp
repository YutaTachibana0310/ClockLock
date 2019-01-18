//=============================================================================
//
// ゲームオーバー処理 [gameOver.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "gameOver.h"
#include "transition.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define GAMEOVER_DIFFUSE_ALPHA			(3)
#define GAMEOVER_FADEIN_FRAMEMAX		(85)
#define GAMEOVER_TRANSITION_FRAMEMAX	(120)
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexGameOver(void);				//頂点作成関数
void SetTextureGameOver(void);	// テクスチャ座標の計算処理
void SetVertexGameOver(void);					// 頂点の計算処理
void SetDiffuseGameOver(void);

/*****************************************************************************
構造体定義
*****************************************************************************/
enum GAMEOVER_STATE
{
	GAMEOVER_FADEIN,
	GAMEOVER_TRANSITION,
	GAMEOVER_STATEMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

GAMEOVER gameOver;										//ゲームオーバー

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitGameOver(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAMEOVER* ptr = &gameOver;

	ptr->pos = D3DXVECTOR3(GAMEOVER_INITPOS_X, GAMEOVER_INITPOS_Y, 0.0f);

	ptr->angle = atan2f(GAMEOVER_TEXTURE_SIZE_Y, GAMEOVER_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(GAMEOVER_TEXTURE_SIZE_X, GAMEOVER_TEXTURE_SIZE_Y));

	ptr->active = false;
	ptr->state = GAMEOVER_FADEIN;

	// 頂点情報の作成
	MakeVertexGameOver();
	SetTextureGameOver();
	SetVertexGameOver();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)GAMEOVER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitGameOver(void)
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
void UpdateGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	if (!ptr->active)
	{
		return;
	}

	if (ptr->state == GAMEOVER_FADEIN)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMEOVER_FADEIN_FRAMEMAX);
		if (ptr->cntFrame == GAMEOVER_FADEIN_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMEOVER_TRANSITION;
		}
	}
	else if (ptr->state == GAMEOVER_TRANSITION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMEOVER_TRANSITION_FRAMEMAX);
		if (ptr->cntFrame == GAMEOVER_TRANSITION_FRAMEMAX)
		{
			SetTransition(SCENE_NAMEENTRY);
			ptr->state = GAMEOVER_STATEMAX;
		}
	}


}

/******************************************************************************
描画処理
******************************************************************************/
void DrawGameOver(void)
{
	if (!gameOver.active)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//diffuseを設定
	SetDiffuseGameOver();

	//テクスチャブレンディングを設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//テクスチャブレンディングを再設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexGameOver(void)
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
void SetTextureGameOver(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

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
ディフューズの設定
******************************************************************************/
void SetDiffuseGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	if (ptr->state != GAMEOVER_FADEIN)
	{
		return;
	}

	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, ptr->cntFrame * GAMEOVER_DIFFUSE_ALPHA);
}

/******************************************************************************
//関数名	：GAMEOVER *GetGameOverAdr(int pno)
//引数		：int pno：取得したいゲームオーバーのインデックス
//戻り値	：正常な引数：ゲームオーバーのアドレス	異常な引数：NULL
//説明		：ゲームオーバーのアドレス取得関数
******************************************************************************/
GAMEOVER *GetGameOverAdr(void)
{

	return &gameOver;
}

/******************************************************************************
//関数名	：void SetGameOver(void)
//引数		：void
//戻り値	：void
//説明		：ゲームオーバーのset関数
******************************************************************************/
void SetGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	ptr->active = true;
	ptr->cntFrame = 0;
	SetScene(SCENE_GAMEOVER);
}