//=============================================================================
//
// GUI処理 [GUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "GUI.h"
#include "Score.h"
#include "scoreItemGUI.h"
#include "zankiGUI.h"
#include "energyGUI.h"
#include "hiScoreGUI.h"
#include "bomGUI.h"
#include "comboGUI.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define GUI_QUAKE_FRAME			(60)					//GUIが揺れる時間

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexGUIback(void);						// GUI背景の頂点作成
void SetTextureGUIback(void);							// GUI背景のテクスチャセット
void SetVertexGUIback(D3DXVECTOR3 pos);					// 頂点の計算処理

/*****************************************************************************
 グローバル変数
*****************************************************************************/
static VERTEX_2D				vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク	
static LPDIRECT3DTEXTURE9		texture = NULL;		// テクスチャへのポインタ
static float					g_fRadius;					// ポリゴンの半径
static float					g_fBaseAngle;				// ポリゴンの角度

GUIback back;												// GUI背景

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUIback *back = GetGUIbackAdr();

	//背景の左について初期化
	back->posL = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);

	//背景右について初期化
	back->posR = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X + GUI_TEXTURE_SIZE_X, 0.0f, 0.0f);

	// 頂点情報の作成
	MakeVertexGUIback();

	back->state = GUI_DEFAULT;

	if (num == 0)
	{
		texture = CreateTextureFromFile((LPSTR)GUI_TEXTURE_BACK, pDevice);
	}

	InitScore(num);
	InitScoreItemGUI(num);
	InitZankiGUI(num);
	InitEnergyGUI(num);
	InitHiScoreGUI(num);
	InitBomGUI(num);
	InitComboGUI(num);

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitGUI(void)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}

	UninitScore();
	UninitScoreItemGUI();
	UninitZankiGUI();
	UninitEnergyGUI();
	UninitHiScoreGUI();
	UninitBomGUI();
	UninitComboGUI();
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateGUI(void)
{
	//QUAKE状態のGUIのときはフレームをカウント
	GUIback *ptr = &back;
	if (ptr->state == GUI_QUAKE)
	{
		ptr->cntFrame++;

		if (ptr->cntFrame == GUI_QUAKE_FRAME)
		{
			ptr->state = GUI_DEFAULT;
		}
	}

	//各GUIの更新
	UpdateScore();
	UpdateScoreItemGUI();
	UpdateZankiGUI();
	UpdateEnergyGUI();
	UpdateHiScoreGUI();
	UpdateBomGUI();
	UpdateComboGUI();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUIback *back = GetGUIbackAdr();
	pDevice->SetTexture(0, texture);

	// αブレンドを行わない
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点情報の設定
	SetVertexGUIback(back->posR);

	// 背景右の描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	// 頂点情報の設定
	SetVertexGUIback(back->posL);

	// 背景左の描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//DrawScoreBack();
	DrawScoreItemGUIBack();
	//DrawZankiGUIBack();
	DrawEnergyGUIBack();
	DrawHiScoreGUIBack();
	//DrawBomGUIBack();
	DrawComboGUIBack();

	//αブレンドを再設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	DrawScoreItemGUI();

	DrawEnergyGUI();
	DrawHiscoreNum();
	DrawComboGUI();
}

/******************************************************************************
ゲーム画面上のパラメータ表示
******************************************************************************/
void DrawParameter(void)
{
	DrawScore();
	DrawZankiGUI();
	DrawBomGUI();
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexGUIback(void)
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

	// テクスチャ座標の設定
	SetTextureGUIback();

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureGUIback(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexGUIback(D3DXVECTOR3 pos)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x - GUI_TEXTURE_SIZE_X, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + GUI_TEXTURE_SIZE_Y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x - GUI_TEXTURE_SIZE_X, pos.y + GUI_TEXTURE_SIZE_Y, pos.z);
}

/****************************************************************
関数名	：GUIback *GetGUIbackAdr(void)
説明	：GUI背景アドレス取得関数
引数	：voidint pno：アドレスを取得したいGUI背景のインデックス
戻り値	：GUIbackのアドレス
*****************************************************************/
GUIback *GetGUIbackAdr(void)
{
	return &back;
}

/****************************************************************
関数名	：void SetGUIShake(void)
説明	：GUIを揺らす状態にセットする関数
引数	：void
戻り値	：void
*****************************************************************/
void SetGUIShake(void)
{
	GUIback *ptr = &back;

	ptr->cntFrame = 0;
	ptr->state = GUI_QUAKE;
}