//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "bgObj.h"
#include "enemyBullet.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/


/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexBG(void);						// 頂点情報作成
void SetTextureBG(BG *ptrBG);					// テクスチャアニメーション 
void SetVertexBG(void);							// 頂点の計算処理
BG *GetBGAdr(int pno);							// 背景アドレス取得関数

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9	timeEffect;							//時間操作中のエフェクトテクスチャ
static VERTEX_2D			vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
static VERTEX_2D			effectVertexWk[NUM_VERTEX];			//時間操作エフェクト用頂点情報
static BG					bg[BG_MAX];							// BG構造体
static int					cntTimeEffect = 0;					//時間操作が開始されてからの経過フレーム

/******************************************************************************
/ 初期化処理
*****************************************************************************/
HRESULT InitBG(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BG *ptrBG = GetBGAdr(0);

	for (int i = 0; i < BG_MAX; i++, ptrBG++)
	{
		ptrBG->offsetX = 0.0f;
		ptrBG->offsetY = 0.0f;
		// 頂点情報の作成
		MakeVertexBG();
	}

	ptrBG = GetBGAdr(0);

	if (num == 0)
	{
		// テクスチャの読み込み
		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_0, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED1;
		ptrBG++;

		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_1, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED2;
		ptrBG++;

		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_2, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED3;

		timeEffect = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_EFFECT1, pDevice);
	}

	InitBgObj(num);

	return S_OK;
}

/******************************************************************************
// 終了処理
******************************************************************************/
void UninitBG(void)
{
	int i;
	BG *ptrBG = GetBGAdr(0);

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		if (ptrBG->tex != NULL)
		{	// テクスチャの開放
			ptrBG->tex->Release();
			ptrBG->tex = NULL;
		}
	}

	UninitBgObj();
}

/******************************************************************************
// 更新処理
******************************************************************************/
void UpdateBG(void)
{
	BG *ptrBG = GetBGAdr(0);
	int i;

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		ptrBG->offsetY += ptrBG->offsetSpeedY;

		if (ptrBG->offsetX > 1.0f)
		{
			ptrBG->offsetX -= 1.0f;
		}
		if (ptrBG->offsetY > 1.0f)
		{
			ptrBG->offsetY -= 1.0f;
		}
	}

	UpdateBgObj();
}

/********************************************************************************
// 描画処理
*******************************************************************************/
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BG *ptrBG = GetBGAdr(0);
	SPEEDTYPE type = GetSpeedType();
	int i;

	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, ptrBG->tex);

		//テクスチャ座標の設定
		SetTextureBG(ptrBG);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &vertexWk, sizeof(VERTEX_2D));
	}

	DrawBgObj();
	DrawTimeEffect();
}

/********************************************************************************
// 描画処理(タイムエフェクト)
*******************************************************************************/
void DrawTimeEffect(void)
{
	SPEEDTYPE type = GetSpeedType();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetTimeEffect(type);
	//時間操作発動中であればエフェクト用の背景を描画
	if (type != DEFAULT_SPEED)
	{
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->SetTexture(0, timeEffect);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, effectVertexWk, sizeof(VERTEX_2D));
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

/******************************************************************************
// 頂点の作成
*******************************************************************************/
HRESULT MakeVertexBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	effectVertexWk[0].rhw =
		effectVertexWk[1].rhw =
		effectVertexWk[2].rhw =
		effectVertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点座標の設定
	SetVertexBG();


	return S_OK;
}

/******************************************************************************
// テクスチャ座標の設定
******************************************************************************/
void SetTextureBG(BG *ptrBG)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(ptrBG->offsetX, ptrBG->offsetY);
	vertexWk[1].tex = D3DXVECTOR2(ptrBG->offsetX + 1.0f, ptrBG->offsetY);
	vertexWk[2].tex = D3DXVECTOR2(ptrBG->offsetX, ptrBG->offsetY + 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(ptrBG->offsetX + 1.0f, ptrBG->offsetY + 1.0f);

}


/*******************************************************************************
// 頂点座標の設定
*******************************************************************************/
void SetVertexBG(void)
{
	/*背景は回転させないので左上の頂点を基準にして頂点座標を設定する*/
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);

	effectVertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	effectVertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	effectVertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	effectVertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);
}

/****************************************************************
関数名	：BG *GetBGAdr(int pno)
説明	：背景アドレス取得関数
引数	：int pno：アドレスを取得したいBGのインデックス
戻り値	：bg[i]のアドレス
*****************************************************************/
BG *GetBGAdr(int pno)
{
	return &bg[pno];
}

/****************************************************************
関数名	：void SetTimeEffect(void)
説明	：時間操作エフェクト背景のセット関数
引数	：
戻り値	：void
*****************************************************************/
void SetTimeEffect(SPEEDTYPE type)
{
	//頂点の反射光の設定
	cntTimeEffect += 10;
	float tmp = (cntTimeEffect + 1 > 100) ? 1.0f : cntTimeEffect / 100.0f;
	if (type == SLOW_ORANGE)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA((int)(255 - tmp * 78), 100, 0, 200);
	}
	else if (type == SLOW_GREEN)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(0, (int)(255 - tmp * 78), 0, 200);
	}
	else if (type == SLOW_BOTH)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, (int)(255 - tmp * 78), 200);
	}
	else
	{
		cntTimeEffect = 0;
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//テクスチャ座標を設定
	effectVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	effectVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	effectVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	effectVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
