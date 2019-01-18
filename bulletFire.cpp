//***************************************************************************==
//
// バレットファイア処理 [bulletFire.cpp]
// Author : GP11A341 21 立花雄太
//
//***************************************************************************==
#include "main.h"
#include "bulletFire.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLETFIRE_INITPOS_X						(-100)
#define BULLETFIRE_INITPOS_Y						(-100)

#define EXPLOSIONMIDDLE_SIZE_LENGTH				(51)					//爆発のテクスチャサイズの乱数幅
#define EXPLOSIONMIDDLE_INITSPEED				(3.0f)					//中型爆発パーティクルの初期スピード
#define EXPLOSIONMIDDLE_SPEED_LENGTH			(60)					//爆発のスピードの乱数幅

#define ENEMYPIECE_SIZE_LENFTH					(15)					//エネミーの破片サイズの乱数幅
#define ENEMYPIECE_SPEED_LENGTH					(50)					//エネミーの破片のスピードの乱数幅
#define ENEMYPIECE_INITSPEED					(50.0f)					//エネミーの破片の初期スピード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletFire(void);									//頂点の作成
void SetTextureBulletFire(int cntPattern, BULLETFIRE *ptr);			//テクスチャの計算処理
void SetVertexBulletFire(BULLETFIRE *ptr);							//頂点の計算処理

/*****************************************************************************
グローバル変数
*****************************************************************************/
static VERTEX_2D				vertexWk[NUM_VERTEX];				// 頂点情報の
static LPDIRECT3DTEXTURE9		textureBulletFire = NULL;			// バレットファイアテクスチャへのポインタ

static float					g_fRadius;							// ポリゴンの半径
static float					g_fBaseAngle;						// ポリゴンの角度

BULLETFIRE						bulletFire[BULLETFIRE_MAX];			// バレットファイア構造体の配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBulletFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *fire;
	int i;

	fire = GetBulletFireAdr(0);

	//バレットファイアの初期化処理
	for (i = 0; i < BULLETFIRE_MAX; i++, fire++)
	{
		fire->pos = D3DXVECTOR3(BULLETFIRE_INITPOS_X, BULLETFIRE_INITPOS_Y, 0.0f);
		fire->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		fire->angle = atan2f(BULLETFIRE_TEXTURE_SIZE_Y, BULLETFIRE_TEXTURE_SIZE_X);
		fire->radius = D3DXVec2Length(&D3DXVECTOR2(BULLETFIRE_TEXTURE_SIZE_X, BULLETFIRE_TEXTURE_SIZE_Y));

		fire->countAnim = 0;
		fire->patternAnim = 0;

		fire->active = false;
	}
	// 頂点情報の作成
	MakeVertexBulletFire();

	if (num == 0)
	{
		// テクスチャの読み込み
		textureBulletFire = CreateTextureFromFile((LPSTR)BULLETFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/***************************************************************************
終了処理
//***************************************************************************/
void UninitBulletFire(void)
{
	if (textureBulletFire != NULL)
	{	// テクスチャの開放
		textureBulletFire->Release();
		textureBulletFire = NULL;
	}
}

//***************************************************************************==
// 更新処理
//***************************************************************************==
void UpdateBulletFire(void)
{
	BULLETFIRE *effect = GetBulletFireAdr(0);
	int i;

	//バレットファイアの更新処理
	for (i = 0; i < BULLETFIRE_MAX; i++, effect++)
	{
		if (effect->active)
		{
			// アニメーション
			effect->countAnim++;

			// アニメーションWaitチェック
			if (effect->countAnim % TIMEBULLETFIRE_ANIMATION == 0)
			{
				// パターンの切り替え
				effect->patternAnim++;
				//パターンの最後まで切り替えていたら非アクティブに
				if (effect->patternAnim >= BULLETFIRE_ANIM_NUM)
				{
					effect->active = false;
				}
			}
		}
	}
}

//***************************************************************************==
// 描画処理
//***************************************************************************==
void DrawBulletFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *effect = GetBulletFireAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, textureBulletFire);

	//バレットファイアを描画
	for (i = 0; i < BULLETFIRE_MAX; i++, effect++)
	{
		if (effect->active)
		{
			//頂点座標の設定
			SetVertexBulletFire(effect);

			// テクスチャ座標を設定
			SetTextureBulletFire(effect->patternAnim, effect);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//***************************************************************************==
// 頂点の作成
//***************************************************************************==
HRESULT MakeVertexBulletFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *effect = GetBulletFireAdr(0);

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

//***************************************************************************==
// テクスチャ座標の設定
//***************************************************************************==
void SetTextureBulletFire(int cntPattern, BULLETFIRE *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % BULLETFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / BULLETFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BULLETFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BULLETFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


//***************************************************************************==
// 頂点座標の設定
//***************************************************************************==
void SetVertexBulletFire(BULLETFIRE *ptr)
{

	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
//関数名	：BULLETFIRE *GetBulletFireAdr(int pno)
//引数		：int pno:取得したいバレットファイアのインデックス
//戻り値	：void
//説明		：バレットファイアのアドレスを取得する関数
******************************************************************************/
BULLETFIRE *GetBulletFireAdr(int pno)
{
	return &bulletFire[pno];
}

/******************************************************************************
//関数名	：void SetBulletFire(BULLETFIRE *ptr)
//引数		：BULLETFIRE *ptr：セットしたいバレットファイアへのポインタ
//戻り値	：void
//説明		：ゲーム内にバレットファイアをセットする関数
******************************************************************************/
void SetBulletFire(BULLETFIRE *ptr)
{
	if (!ptr->active)
	{
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		SetVertexBulletFire(ptr);
		ptr->active = true;
	}
}
