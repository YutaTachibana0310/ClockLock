//=============================================================================
//
// 残機GUI処理 [zankiGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "zankiGUI.h"
#include "GUI.h"
#include "player.h"
#include "charChip.h"
#include "stageManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/


/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexZankiGUI(void);							// 頂点作成関数
void SetTextureZankiGUI(void);								// テクスチャ座標の計算処理
void SetVertexZankiGUI(float offset);						// 頂点の計算処理（アイコン用）
void SetVertexZankiGUIBack(void);							// 頂点の計算処理（背景用）
void SetVertexZankiGUIChar(void);							// 頂点の計算処理（文字用）
void SetVertexZankiGUINum(float offset, int digitMax);		// 頂点の計算処理（数字用）

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
static LPDIRECT3DTEXTURE9 texture[ZANKIGUI_TEXTURE_NUM];	// テクスチャへのポインタ
static VERTEX_2D		vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

static float angleBack, angleIcon, angleChar;				//ポリゴンの角度
static float radiusBack, radiusIcon, radiusChar;			//ポリゴンの半径
static float radiusNum, angleNum;
static D3DXVECTOR3 basePos;									//基準座標

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitZankiGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//基準座標を設定
	basePos = D3DXVECTOR3(ZANKIGUI_BACK_INITPOS_X, ZANKIGUI_BACK_INITPOS_Y, 0.0f);

	//各パラメータを計算
	angleBack = atan2f(ZANKIGUI_BACKTEX_SIZE_Y, ZANKIGUI_BACKTEX_SIZE_X);
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_BACKTEX_SIZE_X, ZANKIGUI_BACKTEX_SIZE_Y));

	angleIcon = atan2f(ZANKIGUI_TEXTURE_SIZE_Y, ZANKIGUI_TEXTURE_SIZE_X);
	radiusIcon = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_TEXTURE_SIZE_X, ZANKIGUI_TEXTURE_SIZE_Y));

	angleChar = atan2f(ZANKIGUI_CHAR_SIZE_Y, ZANKIGUI_CHAR_SIZE_X);
	radiusChar = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_CHAR_SIZE_X, ZANKIGUI_CHAR_SIZE_Y));

	angleNum = atan2f(ZANKIGUI_NUM_SIZE_Y, ZANKIGUI_NUM_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_NUM_SIZE_X, ZANKIGUI_NUM_SIZE_Y));

	// 頂点情報の作成
	MakeVertexZankiGUI();
	SetTextureZankiGUI();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[BACK] = CreateTextureFromFile((LPSTR)ZANKIGUI_BACKTEX_NAME, pDevice);
		texture[ICON] = CreateTextureFromFile((LPSTR)ZANKIGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitZankiGUI(void)
{
	for (int i = 0; i < ZANKIGUI_TEXTURE_NUM; i++)
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
void UpdateZankiGUI(void)
{
	//揺らす処理
	basePos = D3DXVECTOR3(ZANKIGUI_BACK_INITPOS_X, ZANKIGUI_BACK_INITPOS_Y, 0.0f);

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
void DrawZankiGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayerAdr(0);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[ICON]);

	//アイコンの描画
	SetVertexZankiGUI(0);
	SetTextureZankiGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//×の描画
	SetVertexZankiGUIChar();

	DrawCharChip(vertexWk, CHARCHIP_X);

	//数の描画
	int digitMax = (player->zanki == 0) ? 1 : (int)log10f((float)player->zanki) + 1;
	int count = player->zanki;
	for (int i = 0; i < digitMax; i++, count /= 10)
	{
		SetVertexZankiGUINum(i * ZANKIGUI_NUM_OFFSETPOS, digitMax);
		DrawYellowNum(vertexWk, count % 10);
	}
	
}

/******************************************************************************
描画処理(背景用)
******************************************************************************/
void DrawZankiGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexZankiGUIBack();
	SetTextureZankiGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexZankiGUI(void)
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
void SetTextureZankiGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定(アイコン用)
******************************************************************************/
void SetVertexZankiGUI(float offset)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(ZANKIGUI_INITPOS_X + offset, ZANKIGUI_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_INITPOS_X, ZANKIGUI_INITPOS_Y, 0.0f);
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
void SetVertexZankiGUIBack(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[0].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[1].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[1].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[2].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[2].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
	vertexWk[3].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[3].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
}

/******************************************************************************
頂点座標の設定(文字用)
******************************************************************************/
void SetVertexZankiGUIChar(void)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_CHAR_INITPOS_X, ZANKIGUI_CHAR_INITPOS_Y, 0.0f);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angleChar) * radiusChar;
	vertexWk[0].vtx.y = pos.y - sinf(angleChar) * radiusChar;
	vertexWk[1].vtx.x = pos.x + cosf(angleChar) * radiusChar;
	vertexWk[1].vtx.y = pos.y - sinf(angleChar) * radiusChar;
	vertexWk[2].vtx.x = pos.x - cosf(angleChar) * radiusChar;
	vertexWk[2].vtx.y = pos.y + sinf(angleChar) * radiusChar;
	vertexWk[3].vtx.x = pos.x + cosf(angleChar) * radiusChar;
	vertexWk[3].vtx.y = pos.y + sinf(angleChar) * radiusChar;
}

/******************************************************************************
頂点座標の設定(数字用)
******************************************************************************/
void SetVertexZankiGUINum(float offset, int digitMax)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_NUM_INITPOS_X, ZANKIGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += ZANKIGUI_NUM_OFFSETPOS * (digitMax - 1);
	pos.x -= offset;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum;
}