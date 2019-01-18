//=============================================================================
//
// トランジション処理 [transition.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "transition.h"
#include "sePlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TRANSITION_TAKEFRAME				(10)				//遷移にかけるフレーム数
#define TRANSITION_WAITFRAME				(20)				//待機にかけるフレーム数
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void UpdateTransitionClose(void);								//更新処理（閉まる場合）
void UpdateTransitionWait(void);								//更新処理（閉じた状態）
void UpdateTransitionOpen(void);								//更新処理（開く場合）

HRESULT MakeVertexTransition(void);								//頂点作成関数
void SetTextureTransition(int cntPattern);						// テクスチャ座標の計算処理
void SetVertexTopTransition(float offset);						//頂点の計算処理（上部）
void SetVertexBottomTransition(float offset);					//頂点の計算処理（下部）

/*****************************************************************************
構造体定義
*****************************************************************************/
//遷移時に表示されるポリゴンの定義
enum TRANSITION_SHUTTER
{
	SHUTTER_TOP,
	SHUTTER_BOTTOM,
	SHUTTER_MAX
};

//遷移状態の定義
enum TRANSITION_STATE
{
	TRANSITION_DEFAULT,
	TRANSITION_CLOSE,
	TRANSITION_WAIT,
	TRANSITION_OPEN,
	TRASITIONSTATE_MAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				//テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, radius;								//ポリゴンの角度、半径

static TRANSITION transition;						//トランジション配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTransition(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	angle = atan2f(TRANSITION_TEXTURE_SIZE_Y, TRANSITION_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TRANSITION_TEXTURE_SIZE_X, TRANSITION_TEXTURE_SIZE_Y));

	transition.cntFrame = 0;
	transition.state = TRANSITION_DEFAULT;

	// 頂点情報の作成
	MakeVertexTransition();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TRANSITION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTransition(void)
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
void UpdateTransition(void)
{
	switch (transition.state)
	{
	case TRANSITION_CLOSE:
		UpdateTransitionClose();
		break;

	case TRANSITION_WAIT:
		UpdateTransitionWait();
		break;

	case TRANSITION_OPEN:
		UpdateTransitionOpen();
		break;
	}
}

/******************************************************************************
更新処理(閉まる場合)
******************************************************************************/
void UpdateTransitionClose(void)
{
	transition.cntFrame++;

	if (transition.cntFrame > TRANSITION_TAKEFRAME)
	{
		transition.cntFrame = 0;
		PlaySE(SE_SHUTTER);
		transition.state = TRANSITION_WAIT;
	}
}

/******************************************************************************
更新処理（閉じた状態）
******************************************************************************/
void UpdateTransitionWait(void)
{
	transition.cntFrame++;

	if (transition.cntFrame > TRANSITION_WAITFRAME)
	{
		transition.cntFrame = TRANSITION_TAKEFRAME;
		SetScene(transition.nextScene);
		transition.state = TRANSITION_OPEN;
	}
}

/******************************************************************************
更新処理（開く場合）
******************************************************************************/
void UpdateTransitionOpen(void)
{
	transition.cntFrame--;

	if (transition.cntFrame <= 0)
	{
		transition.cntFrame = 0;
		transition.state = TRANSITION_DEFAULT;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTransition(void)
{
	TRANSITION *ptr = &transition;

	if (ptr->state == TRANSITION_DEFAULT)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float offset, rad;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);


	//トランジションを描画
	if (ptr->state == TRANSITION_WAIT)
	{
		SetVertexTopTransition(SCREEN_CENTER_Y);
		SetTextureTransition(SHUTTER_TOP);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexBottomTransition(-SCREEN_CENTER_Y);
		SetTextureTransition(SHUTTER_BOTTOM);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
	else
	{
		rad = RADIAN((float)ptr->cntFrame / TRANSITION_TAKEFRAME * 90);
		offset = SCREEN_CENTER_Y * sinf(rad);

		SetVertexTopTransition(offset);
		SetTextureTransition(SHUTTER_TOP);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexBottomTransition(-offset);
		SetTextureTransition(SHUTTER_BOTTOM);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));;
	}
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTransition(void)
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
void SetTextureTransition(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TRANSITION_TEXTURE_DIVIDE_X;
	int y = cntPattern / TRANSITION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TRANSITION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TRANSITION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTopTransition(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TRANSITION_INITPOS_X, TRANSITION_TOP_INITPOS_Y, 0.0f);
	pos.y += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = pos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = pos.y + sinf(angle) * radius;
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexBottomTransition(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TRANSITION_INITPOS_X, TRANSITION_BOTTOM_INITPOS_Y, 0.0f);
	pos.y += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = pos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = pos.y + sinf(angle) * radius;
}

/******************************************************************************
//関数名	：TRANSITION *GetTransitionAdr(void)
//引数		：void
//戻り値	：トランジション構造体のアドレス
//説明		：トランジションのアドレス取得関数
******************************************************************************/
TRANSITION *GetTransitionAdr(void)
{
	return &transition;
}

/******************************************************************************
//関数名	：void SetTransition(int nextScene)
//引数		：int nextScene		：遷移するシーン
//戻り値	：void
//説明		：遷移の開始関数
******************************************************************************/
void SetTransition(int nextScene)
{
	transition.nextScene = nextScene;
	transition.cntFrame = 0;
	transition.state = TRANSITION_CLOSE;
}