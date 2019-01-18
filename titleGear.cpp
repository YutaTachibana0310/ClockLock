//=============================================================================
//
// タイトルギア処理 [titleGear.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "titleGear.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TITLEGEAR_SCALE_MAX			(1.3f)			//タイトルギアのスケール最大値
#define TITLEGEAR_SCALE_MIN			(0.7f)			//タイトルギアのスケール最小値
#define TITLEGEAR_ROTVALUE			(RADIAN(0.1f))	//ポリゴンの回転量

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTitleGear(void);				//頂点作成関数
void SetTextureTitleGear(void);					// テクスチャ座標の計算処理
void SetVertexTitleGear(TITLEGEAR *ptr);		// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				//テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, radius;								//ポリゴンの半径、角度

TITLEGEAR titleGear[TITLEGEAR_MAX];						//タイトルギア配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTitleGear(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEGEAR* ptr = GetTitleGearAdr(0);
	int i;

	angle = atan2f(TITLEGEAR_TEXTURE_SIZE_Y, TITLEGEAR_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TITLEGEAR_TEXTURE_SIZE_X, TITLEGEAR_TEXTURE_SIZE_Y));

	// 頂点情報の作成
	MakeVertexTitleGear();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TITLEGEAR_TEXTURE_NAME, pDevice);

		//各パラメータの初期化
		for (i = 0; i < TITLEGEAR_MAX; i++, ptr++)
		{
			ptr->pos = VECTOR3_ZERO;
			ptr->pos.x = RandomRange(0.0f, (float)SCREEN_WIDTH);
			ptr->pos.y = RandomRange(0.0f, (float)SCREEN_HEIGHT);
			ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ptr->scale = RandomRange(TITLEGEAR_SCALE_MIN, TITLEGEAR_SCALE_MAX);
			ptr->countAnim = 0;
			ptr->rotDir = (i % 2 == 0) ? -1 : 1;
		}
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTitleGear(void)
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
void UpdateTitleGear(void)
{
	TITLEGEAR *ptr = &titleGear[0];

	for (int i = 0; i < TITLEGEAR_MAX; i++, ptr++)
	{
		ptr->rot.z += ptr->rotDir * TITLEGEAR_ROTVALUE * ptr->scale;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTitleGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEGEAR *ptr = GetTitleGearAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//タイトルギアを描画
	for (i = 0; i < TITLEGEAR_MAX; i++, ptr++)
	{

		//頂点座標を設定
		SetVertexTitleGear(ptr);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTitleGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);

	SetTextureTitleGear();
	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTitleGear(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTitleGear(TITLEGEAR *ptr)
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
//関数名	：TITLEGEAR *GetTitleGearAdr(int pno)
//引数		：int pno：取得したいタイトルギアのインデックス
//戻り値	：正常な引数：タイトルギアのアドレス	異常な引数：NULL
//説明		：タイトルギアのアドレス取得関数
******************************************************************************/
TITLEGEAR *GetTitleGearAdr(int pno)
{
	if (pno < TITLEGEAR_MAX)
	{
		return &titleGear[pno];
	}
	else
	{
		return NULL;
	}
}