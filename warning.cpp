//=============================================================================
//
// ワーニング処理 [warning.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "warning.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define WARNING_TEXT_MOVELENGTH			(5.0f)	//テキストテクスチャが動く範囲
#define WARNING_ROLL_SCROLLVALUE		(0.005f)	//テクスチャのスクロール量

#define WARNING_OPEN_FRAME				(60)	//OPEN状態から遷移する時間
#define WARNING_OFFSETPOS_MAX			(80.0f)	//offsetPos最大値
#define WARNING_OFFSETPOS_ADDVALUE		(2.0f)	//offsetPos加算値

#define WARNING_DEFAULT_FRAME			(300)	//DEFAULT状態から遷移する時間

#define WARNING_BG_DIFFUSE_MIN			(0)
#define WARNING_BG_DIFFUSE_MAX			(60)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexWarning(void);				//頂点作成関数
void SetTextureWarningText(int num);			//テクスチャ座標の計算処理
void SetTextureWarningRoll(int side);			//テクスチャ座標の計算処理（帯）
void SetTextureWarningBG(void);					//テクスチャ座標の計算処理（背景）
void SetVertexWarningText(void);				//頂点の計算処理
void SetVertexWarningExclamation(void);			//頂点の計算処理
void SetVertexWarningRoll(int side);			//頂点の計算処理（帯）
void SetVertexWarningBG(void);					//頂点の計算処理（背景）

/*****************************************************************************
構造体定義
*****************************************************************************/
enum WARNING_ROLLSIDE
{
	WARNING_TOP,
	WARNING_BOTTOM
};

enum WARNING_TEXTURE
{
	WARNING_EXCLAMATION,
	WARNING_WARNING,
};

enum WARNING_TEXINDEX
{
	WARNING_ROLL,
	WARNING_TEXT,
	WARNING_BG,
	WARNING_TEXMAX
};

enum WARNING_STATE
{
	WARNING_OPEN,
	WARNING_DEFAULT,
	WARNING_STATEMAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[WARNING_TEXMAX];				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

WARNING warning;										//ワーニング構造体

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitWarning(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WARNING* ptr = &warning;
	
	ptr->angle = atan2f(WARNING_TEXTURE_SIZE_Y, WARNING_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(WARNING_TEXTURE_SIZE_X, WARNING_TEXTURE_SIZE_Y));

	ptr->angleRoll = atan2f(WARNING_ROLL_SIZE_Y, WARNING_ROLL_SIZE_X);
	ptr->radiusRoll = D3DXVec2Length(&D3DXVECTOR2(WARNING_ROLL_SIZE_X, WARNING_ROLL_SIZE_Y));

	ptr->angleBG = atan2f(WARNING_BG_SIZE_Y, WARNING_BG_SIZE_X);
	ptr->radiusBG = D3DXVec2Length(&D3DXVECTOR2(WARNING_BG_SIZE_X, WARNING_BG_SIZE_Y));

	ptr->countFrame = 0;
	ptr->offsetPos = 0.0f;
	ptr->offsetUV = 0.0f;
	ptr->countDiffuse = 0;
	ptr->flgDiffuse = true;
	ptr->state = WARNING_OPEN;
	ptr->active = false;


	// 頂点情報の作成
	MakeVertexWarning();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[WARNING_TEXT] = CreateTextureFromFile((LPSTR)WARNING_TEXTURE_NAME, pDevice);
		texture[WARNING_ROLL] = CreateTextureFromFile((LPSTR)WARNING_ROLLTEX_NAME, pDevice);
		texture[WARNING_BG] = CreateTextureFromFile((LPSTR)WARNING_BG_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitWarning(void)
{
	for (int i = 0; i < WARNING_TEXMAX; i++)
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
void UpdateWarning(void)
{
	WARNING *ptr = &warning;

	//非アクティブであれば処理を行わない
	if (!ptr->active)
	{
		return;
	}

	ptr->countFrame++;

	//背景のアルファ計算
	ptr->countDiffuse += (ptr->flgDiffuse) ? 1 : -1;
	if (ptr->countDiffuse == WARNING_BG_DIFFUSE_MAX || ptr->countDiffuse == WARNING_BG_DIFFUSE_MIN)
	{
		ptr->flgDiffuse = !ptr->flgDiffuse;
	}

	//OPEN状態の処理
	if (ptr->state == WARNING_OPEN)
	{
		ptr->offsetPos = Clampf(0.0f, WARNING_OFFSETPOS_MAX, ptr->offsetPos +WARNING_OFFSETPOS_ADDVALUE);
		ptr->offsetUV += WARNING_ROLL_SCROLLVALUE;

		if (ptr->countFrame == WARNING_OPEN_FRAME)
		{
			ptr->state = WARNING_DEFAULT;
			ptr->countFrame = 0;
		}
	}

	//DEFAULT状態の処理
	else if (ptr->state == WARNING_DEFAULT)
	{
		ptr->offsetUV += WARNING_ROLL_SCROLLVALUE;

		if (ptr->countFrame == WARNING_DEFAULT_FRAME)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WARNING *ptr = &warning;

	if (!ptr->active)
	{
		return;
	}

	//背景の描画
	pDevice->SetTexture(0, texture[WARNING_BG]);
	SetTextureWarningBG();
	SetVertexWarningBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//帯を描画
	pDevice->SetTexture(0, texture[WARNING_ROLL]);

	SetVertexWarningRoll(WARNING_TOP);
	SetTextureWarningRoll(WARNING_TOP);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	SetVertexWarningRoll(WARNING_BOTTOM);
	SetTextureWarningRoll(WARNING_BOTTOM);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//テキストを描画(DEFAULT状態のみ)
	if (ptr->state == WARNING_DEFAULT)
	{
		pDevice->SetTexture(0, texture[WARNING_TEXT]);

		SetVertexWarningExclamation();
		SetTextureWarningText(WARNING_EXCLAMATION);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexWarningText();
		SetTextureWarningText(WARNING_TEXT);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexWarning(void)
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
void SetTextureWarningText(int num)
{
	float sizeY = 1.0f / WARNING_TEXTURE_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, num * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, num * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, num * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, num * sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定（帯）
******************************************************************************/
void SetTextureWarningRoll(int side)
{
	WARNING *ptr = &warning;

	vertexWk[0].tex = D3DXVECTOR2(ptr->offsetUV, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(ptr->offsetUV + 1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(ptr->offsetUV, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(ptr->offsetUV + 1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定（背景）
******************************************************************************/
void SetTextureWarningBG()
{
	WARNING *ptr = &warning;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexWarningText(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);
	pos.x += RandomRange(-WARNING_TEXT_MOVELENGTH, WARNING_TEXT_MOVELENGTH);
	pos.y += RandomRange(-WARNING_TEXT_MOVELENGTH, WARNING_TEXT_MOVELENGTH);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexWarningExclamation(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
頂点座標の設定(帯)
******************************************************************************/
void SetVertexWarningRoll(int side)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);
	pos.y += (side == WARNING_TOP) ? ptr->offsetPos : -1 * ptr->offsetPos;
	int sign = (side == WARNING_TOP) ? 1 : -1;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[1].vtx.x = pos.x + sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[2].vtx.x = pos.x - sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[3].vtx.x = pos.x + sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleRoll) * ptr->radiusRoll;

}

/******************************************************************************
頂点座標の設定(背景)
******************************************************************************/
void SetVertexWarningBG(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_BG_INITPOS_X, WARNING_BG_INITPOS_Y, 0.0f);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleBG) * ptr->radiusBG;

	int alpha = (int)(255 * (float)ptr->countDiffuse / WARNING_BG_DIFFUSE_MAX);
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

/******************************************************************************
//関数名	：WARNING *GetWarningAdr(int pno)
//引数		：void
//戻り値	：正常な引数：ワーニングのアドレス	異常な引数：NULL
//説明		：ワーニングのアドレス取得関数
******************************************************************************/
WARNING *GetWarningAdr(void)
{
	return &warning;
}

/******************************************************************************
//関数名	：void SetWarning(void)
//引数		：void
//戻り値	：void
//説明		；ワーニングのセット関数
******************************************************************************/
void SetWarning(void)
{
	WARNING *ptr = &warning;

	ptr->countFrame = 0;

	ptr->offsetPos = 0.0f;
	ptr->offsetUV = 0.0f;
	ptr->state = WARNING_OPEN;

	ptr->active = true;
}