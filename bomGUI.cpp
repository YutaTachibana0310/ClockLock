//=============================================================================
//
// ボムGUI処理 [bomGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "bomGUI.h"
#include "player.h"
#include "stageManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BOMGUI_SPEED (-40.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexBomGUI(void);				// 頂点作成関数
void SetTextureBomGUI(void);					// テクスチャ座標の計算処理
void SetVertexBomGUI(float offset);			// 頂点の計算処理（アイコン用）
void SetVertexBomGUIBack(void);				// 頂点の計算処理（背景用）

/*****************************************************************************
構造体定義
*****************************************************************************/
enum 
{
	BACK,
	ICON
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[BOMGUI_TEXTURE_NUM];	// テクスチャへのポインタ
static VERTEX_2D		vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

static float angleBack, angleIcon;							//ポリゴンの角度
static float radiusBack, radiusIcon;						//ポリゴンの半径

static D3DXVECTOR3 basePos;									//基準座標

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBomGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各パラメータを計算
	angleBack = atan2f(BOMGUI_BACKTEX_SIZE_Y, BOMGUI_BACKTEX_SIZE_X);
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(BOMGUI_BACKTEX_SIZE_X, BOMGUI_BACKTEX_SIZE_Y));

	angleIcon = atan2f(BOMGUI_TEXTURE_SIZE_Y, BOMGUI_TEXTURE_SIZE_X);
	radiusIcon = D3DXVec2Length(&D3DXVECTOR2(BOMGUI_TEXTURE_SIZE_X, BOMGUI_TEXTURE_SIZE_Y));

	// 頂点情報の作成
	MakeVertexBomGUI();
	SetTextureBomGUI();

	//基準座標を設定
	basePos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);

	if(num == 0)
	{
		// テクスチャの読み込み
		texture[BACK] = CreateTextureFromFile((LPSTR)BOMGUI_BACKTEX_NAME, pDevice);
		texture[ICON] = CreateTextureFromFile((LPSTR)BOMGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBomGUI(void)
{
	for (int i = 0; i < BOMGUI_TEXTURE_NUM; i++)
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
void UpdateBomGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);
	if (GetStageManagerAdr()->state == STAGE_BOSSBATTLE)
	{
		basePos.y += BOSSGAUGE_OFFSET;
	}

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBomGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayerAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[ICON]);

	for (i = 0; i < player->bomCnt; i++)
	{
		SetVertexBomGUI(BOMGUI_OFFSETPOS * i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
描画処理(背景用)
******************************************************************************/
void DrawBomGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexBomGUIBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexBomGUI(void)
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
void SetTextureBomGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定(アイコン用)
******************************************************************************/
void SetVertexBomGUI(float offset)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(BOMGUI_INITPOS_X + offset, BOMGUI_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(BOMGUI_INITPOS_X, BOMGUI_INITPOS_Y, 0.0f);
	pos.x += offset;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angleIcon) * radiusIcon;
	vertexWk[0].vtx.y = pos.y - sinf(angleIcon) * radiusIcon;
	vertexWk[1].vtx.x = pos.x + cosf(angleIcon) * radiusIcon;
	vertexWk[1].vtx.y = pos.y - sinf(angleIcon) * radiusIcon;
	vertexWk[2].vtx.x = pos.x - cosf(angleIcon) * radiusIcon;
	vertexWk[2].vtx.y = pos.y + sinf(angleIcon) * radiusIcon;
	vertexWk[3].vtx.x = pos.x + cosf(angleIcon) * radiusIcon;
	vertexWk[3].vtx.y = pos.y + sinf(angleIcon) * radiusIcon;
}

/******************************************************************************
頂点座標の設定(背景用)
******************************************************************************/
void SetVertexBomGUIBack(void)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angleBack) * radiusBack;
	vertexWk[0].vtx.y = pos.y - sinf(angleBack) * radiusBack;
	vertexWk[1].vtx.x = pos.x + cosf(angleBack) * radiusBack;
	vertexWk[1].vtx.y = pos.y - sinf(angleBack) * radiusBack;
	vertexWk[2].vtx.x = pos.x - cosf(angleBack) * radiusBack;
	vertexWk[2].vtx.y = pos.y + sinf(angleBack) * radiusBack;
	vertexWk[3].vtx.x = pos.x + cosf(angleBack) * radiusBack;
	vertexWk[3].vtx.y = pos.y + sinf(angleBack) * radiusBack;
}