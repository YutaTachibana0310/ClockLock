//=============================================================================
//
// ポーズメニュー処理 [pauseMenu.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "pauseMenu.h"
#include "input.h"
#include "transition.h"
/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PAUSEMENU_OPEN_FRAME		(10)
#define PAUSEMENU_CLOSE_FRAME		(10)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexPauseMenu(void);							//頂点作成関数
void SetTexturePauseMenu(void);								// テクスチャ座標の計算処理
void SetTexturePauseCursor(int cntPattern);					//テクスチャ座標の計算処理（カーソル用）
void SetVertexPauseMenu(PAUSEMENU *ptr);					// 頂点の計算処理
void UpdatePauseMenuOpen(PAUSEMENU *ptr);					//更新処理
void UpdatePauseMenuSelect(PAUSEMENU *ptr);					//更新処理
void UpdatePauseMenuClose(PAUSEMENU *ptr);					//更新処理
void ProcessSelectedPauseMenu(PAUSEMENU *ptr);				//メニュー処理
void DrawPauseMenuBack(PAUSEMENU *ptr);						//描画処理（背景用）
void DrawPauseMenuCursor(PAUSEMENU *ptr);					//描画処理（カーソル用）

/*****************************************************************************
構造体定義
*****************************************************************************/
//ポーズメニューの選択メニュー列挙型
enum PAUSE_MENU
{
	MENU_RESUME,
	MENU_RETRY,
	MENU_RETURN,
	MENU_MAX
};

//ポーズメニューのテクスチャ列挙体
enum PAUSEMENU_TEXTURE
{
	BACK,
	CURSOR,
	TEXTURE_MAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TEXTURE_MAX];				// テクスチャへのポインタ

static VERTEX_2D vertexWk[NUM_VERTEX];						//頂点情報格納ワーク
static float angle, radius;									//ポリゴンの半径、角度

PAUSEMENU pauseMenu[PAUSEMENU_MAX];							//ポーズメニュー配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPauseMenu(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU* ptr = GetPauseMenuAdr(0);
	int i;

	for (i = 0; i < PAUSEMENU_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PAUSEMENU_INITPOS_X, PAUSEMENU_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		angle = atan2f(PAUSEMENU_TEXTURE_SIZE_Y, PAUSEMENU_TEXTURE_SIZE_X);
		radius = D3DXVec2Length(&D3DXVECTOR2(PAUSEMENU_TEXTURE_SIZE_X, PAUSEMENU_TEXTURE_SIZE_Y));

		ptr->state = PAUSEMENU_NONE;
		ptr->scale = 0.0f;
		ptr->select = MENU_RESUME;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexPauseMenu();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[BACK] = CreateTextureFromFile((LPSTR)PAUSEMENU_TEXTURE_NAME, pDevice);
		texture[CURSOR] = CreateTextureFromFile((LPSTR)PAUSEMENU_CURSORTEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPauseMenu(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
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
void UpdatePauseMenu(void)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);

	ptr->cntFrame++;
	switch (ptr->state)
	{
	case PAUSEMENU_OPEN:
		UpdatePauseMenuOpen(ptr);
		break;

	case PAUSEMENU_SELECT:
		UpdatePauseMenuSelect(ptr);
		break;

	case PAUSEMENU_CLOSE:
		UpdatePauseMenuClose(ptr);
		break;
	}

}

/******************************************************************************
更新処理（OPEN状態）
******************************************************************************/
void UpdatePauseMenuOpen(PAUSEMENU *ptr)
{
	if (ptr->cntFrame > PAUSEMENU_OPEN_FRAME)
	{
		ptr->state = PAUSEMENU_SELECT;
		return;
	}

	ptr->scale += 1.0f / PAUSEMENU_OPEN_FRAME;
}

/******************************************************************************
更新処理（SELECT状態）
******************************************************************************/
void UpdatePauseMenuSelect(PAUSEMENU *ptr)
{
	ptr->select = WrapAround(MENU_RESUME, MENU_MAX, ptr->select - GetMyAxisYTrigger(0));

	if (GetMyButton0Trigger(0))
	{
		ptr->state = PAUSEMENU_CLOSE;
		ptr->cntFrame = 0;
	}
}

/******************************************************************************
更新処理（CLOSE状態）
******************************************************************************/
void UpdatePauseMenuClose(PAUSEMENU *ptr)
{
	if (ptr->cntFrame > PAUSEMENU_CLOSE_FRAME)
	{
		ptr->state = PAUSEMENU_NONE;
		ptr->cntFrame = 0;

		ProcessSelectedPauseMenu(ptr);
		return;
	}

	ptr->scale -= 1.0f / PAUSEMENU_CLOSE_FRAME;
}

/******************************************************************************
選択されたメニューの処理
******************************************************************************/
void ProcessSelectedPauseMenu(PAUSEMENU *ptr)
{
	switch (ptr->select)
	{
	case MENU_RESUME:
		Pause(false);
		ResumeGame();
		break;

	case MENU_RETRY:
		Pause(false);
		//StartStage(0);
		SetTransition(SCENE_GAME);
		break;

	case MENU_RETURN:
		Pause(false);
		SetTransition(SCENE_TITLE);
		break;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawPauseMenu(void)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);

	DrawPauseMenuBack(ptr);
	DrawPauseMenuCursor(ptr);
}

/******************************************************************************
更新処理（背景用）
******************************************************************************/
void DrawPauseMenuBack(PAUSEMENU *ptr)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexPauseMenu(ptr);
	SetTexturePauseMenu();
	
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
更新処理（カーソル用）
******************************************************************************/
void DrawPauseMenuCursor(PAUSEMENU *ptr)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture[CURSOR]);

	SetTexturePauseCursor(ptr->select);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexPauseMenu(void)
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
void SetTexturePauseMenu(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定（カーソル用）
******************************************************************************/
void SetTexturePauseCursor(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % PAUSEMENU_CURSORTEX_DIVIDE_X;
	float sizeX = 1.0f / PAUSEMENU_CURSORTEX_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexPauseMenu(PAUSEMENU *ptr)
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
//関数名	：PAUSEMENU *GetPauseMenuAdr(int pno)
//引数		：int pno：取得したいポーズメニューのインデックス
//戻り値	：正常な引数：ポーズメニューのアドレス	異常な引数：NULL
//説明		：ポーズメニューのアドレス取得関数
******************************************************************************/
PAUSEMENU *GetPauseMenuAdr(int pno)
{
	if (pno < PAUSEMENU_MAX)
	{
		return &pauseMenu[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetPauseMenustate(PAUSEMENU_STATE state)
//引数		：PAUSEMENU_STATE state		:セットしたいポーズメニューの状態
//戻り値	：void
//説明		：ポーズメニューの状態セット関数
******************************************************************************/
void SetPauseMenuState(PAUSEMENU_STATE state)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);
	ptr->cntFrame = 0;
	ptr->state = state;
}