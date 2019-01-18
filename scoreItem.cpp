//=============================================================================
//
// スコアアイテム処理 [scoreItem.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bg.h"
#include "scoreItem.h"
#include "sePlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SCOREITEM_SCALEUP_FRAME				(30)				//スコアアイテムのポップにかけるフレーム
#define SCOREITEM_SPEED						(7.0f)				//スコアアイテムのスピード
#define SCOREITEM_SPEED_HOMING				(15.0f)				//スコアアイテムのスピード（追尾状態）
#define SCOREITEM_LENGTH_HOMING				(500*500)			//スコアアイテムが追尾状態に遷移する距離

#define SCOREITEM_SCALE_MAX					(1.5f)				//スケール最大値
#define SCOREITEM_SCALE_MIN					(1.0f)				//スケール最小値

/*****************************************************************************
構造体定義
*****************************************************************************/
enum SCOREITEM_STATE
{
	SCOREITEM_POP,
	SCOREITEM_DOWN,
	SCOREITEM_HOMING
};

enum SCALE_STATE
{
	DEFAULT,
	ENLARGEMENT,
	REDUCTION
};
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexScoreItem(void);							//頂点作成関数
void SetTextureScoreItem(int cntPattern, SCOREITEM *ptr);	// テクスチャ座標の計算処理
void SetVertexScoreItem(SCOREITEM *ptr);					// 頂点の計算処理

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SCOREITEM_TEXTURE_NUM];			// テクスチャへのポインタ（メイン用）

SCOREITEM scoreItem[SCOREITEM_MAX];				//スコアアイテム配列(メインショット)

static VERTEX_2D vertexWk[NUM_VERTEX];			//頂点情報格納ワーク
static int getItemCount;						//アイテム取得数
static int scaleState;
static float scale;


#ifdef _DEBUG
static int				cntScoreItem = 0;						//アクティブなスコアアイテムの数
#endif

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScoreItem(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREITEM* ptr = GetScoreItemAdr(0);
	int i;

	getItemCount = 0;
	scaleState = DEFAULT;
	scale = 1.0f;

	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SCOREITEM_INITPOS_X, SCOREITEM_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SCOREITEM_TEXTURE_SIZE_Y, SCOREITEM_TEXTURE_SIZE_Y);
		ptr->baseRadius = ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SCOREITEM_TEXTURE_SIZE_X, SCOREITEM_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexScoreItem();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[0] = CreateTextureFromFile((LPSTR)SCOREITEMLUMINE_TEXTURE_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)SCOREITEM_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScoreItem(void)
{
	for(int i = 0; i < SCOREITEM_TEXTURE_NUM; i++)
	if (texture[i] != NULL)
	{	// テクスチャの開放
		texture[i]->Release();
		texture[i] = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateScoreItem(void)
{
	SCOREITEM *ptr = GetScoreItemAdr(0);
	PLAYER *player = GetPlayerAdr(0);
	D3DXVECTOR3 tmp;								//プレイヤーとアイテムの距離を求めるのに使用するベクトル
	int i;
	float length = SCOREITEM_LENGTH_HOMING + 1.f;	//プレイヤーとアイテムの距離

	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		//非アクティブのアイテムであれば処理を行わない
		if (!ptr->active)
		{
			continue;
		}

		//アイテムの状態によって処理を分岐
		switch (ptr->state)
		{
			//画面内にセットされた直後の状態
		case SCOREITEM_POP:
			ptr->cntFrame++;

			//徐々に減速しながら上方向へポップし拡大
			ptr->pos += ptr->velocity * SCOREITEM_SPEED * (float)(SCOREITEM_SCALEUP_FRAME - ptr->cntFrame) / SCOREITEM_SCALEUP_FRAME;
			ptr->radius = ptr->baseRadius * ptr->cntFrame / SCOREITEM_SCALEUP_FRAME;

			//ポップが終わったら次の状態へ遷移
			if (ptr->cntFrame >= SCOREITEM_SCALEUP_FRAME)
			{
				ptr->state = SCOREITEM_DOWN;
				ptr->velocity = VECTOR3_UP * -1;
			}
			break;

			//画面内を上から下へ移動する状態
		case SCOREITEM_DOWN:
			ptr->pos += ptr->velocity * SCOREITEM_SPEED;

			//プレイヤーとの距離を計算し近かったら追尾状態へ遷移
			if (player->active)
			{
				tmp = player->pos - ptr->pos;
				length = D3DXVec3LengthSq(&tmp);
				if (length < SCOREITEM_LENGTH_HOMING)
				{
					ptr->state = SCOREITEM_HOMING;
				}
			}
			break;

			//近くのプレイヤーの方向へ向かって移動する状態
		case SCOREITEM_HOMING:
			tmp = player->pos - ptr->pos;
			D3DXVec3Normalize(&ptr->velocity, &tmp);

			ptr->pos += ptr->velocity * SCOREITEM_SPEED_HOMING;

			ptr->scale = Maxf(0.2f, ptr->scale - 0.01f);
			//プレイヤーが非アクティブであればDOWN状態へ遷移
			if(!player->active)
			{
				ptr->state = SCOREITEM_DOWN;
			}
			break;
		}

		//更新後の座標が画面外であれば非アクティブに
		if (ptr->pos.x + SCOREITEM_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - SCOREITEM_TEXTURE_SIZE_X > BG_RIGHT)
		{
			ptr->active = false;
#ifdef _DEBUG
			cntScoreItem--;
#endif
		}
		else if (ptr->pos.y + SCOREITEM_TEXTURE_SIZE_Y < 0 || ptr->pos.y - SCOREITEM_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
		{
			ptr->active = false;
#ifdef _DEBUG
			cntScoreItem--;
#endif
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % TIMESCOREITEM_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_SCOREITEM_NUM;
		}
	}

	//スケールの設定
	if (scaleState == ENLARGEMENT)
	{
		scale += 0.1f;
		if (scale >= SCOREITEM_SCALE_MAX)
		{
			scaleState = REDUCTION;
		}
	}
	else if (scaleState == REDUCTION)
	{
		scale -= 0.1f;
		if (scale <= SCOREITEM_SCALE_MIN)
		{
			scaleState = DEFAULT;
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawScoreItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREITEM *ptr = GetScoreItemAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, texture[0]);

	//スコアアイテムの光を描画
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexScoreItem(ptr);

			// テクスチャ座標を設定
			SetTextureScoreItem(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//スコアアイテムを描画
	ptr = GetScoreItemAdr(0);
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexScoreItem(ptr);

			// テクスチャ座標を設定
			SetTextureScoreItem(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SCOREITEM_BB_LEFT, SCOREITEM_BB_TOP, 0.0f), SCOREITEM_BB_WIDTH, SCOREITEM_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexScoreItem(void)
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
void SetTextureScoreItem(int cntPattern, SCOREITEM *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % SCOREITEM_TEXTURE_DIVIDE_X;
	int y = cntPattern / SCOREITEM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCOREITEM_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SCOREITEM_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexScoreItem(SCOREITEM *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//関数名	：SCOREITEM *GetScoreItemAdr(int pno)
//引数		：int pno：取得したいスコアアイテムのインデックス
//戻り値	：正常な引数：スコアアイテムのアドレス	異常な引数：NULL
//説明		：スコアアイテムのアドレス取得関数（メインショット）
******************************************************************************/
SCOREITEM *GetScoreItemAdr(int pno)
{
	if (pno < SCOREITEM_MAX)
	{
		return &scoreItem[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetScoreItem(D3DXVECTOR3 pos, float range, int num)
//引数1		：D3DXVECTOR3 pos	：スコアアイテムをセットしたい基準座標
//引数2		：float range		：基準座標からのセット範囲
//引数3		：int num			：セットしたいアイテムの数
//戻り値	：void
//説明		：スコアアイテムのセット関数
******************************************************************************/
void SetScoreItem(D3DXVECTOR3 pos, float range, int num)
{
	int i, j;
	SCOREITEM *ptr = GetScoreItemAdr(0);

	for (i = 0; i < num; i++, ptr = GetScoreItemAdr(0))
	{
		for (j = 0; j < SCOREITEM_MAX; j++, ptr++)
		{
			//非アクティブなアイテムがあればセット
			if (!ptr->active)
			{
				ptr->pos = pos + D3DXVECTOR3(RandomRange(-1 * range, range), RandomRange(-1 * range, range), 0.0f);
				ptr->velocity = VECTOR3_UP;
				ptr->cntFrame = 0;
				ptr->patternAnim = 0;
				ptr->countAnim = 0;
				ptr->state = SCOREITEM_POP;
				ptr->patternAnim = rand() % ANIM_SCOREITEM_NUM;
				ptr->scale = 1.0f;
				ptr->active = true;
#ifdef _DEBUG
				cntScoreItem++;
#endif
				break;
			}
		}
	}
}

/******************************************************************************
//関数名	：int GetScoreItemCount(void)
//引数1		：void
//戻り値	：int		：取得したスコアアイテムの数
//説明		：取得したスコアアイテムの数のゲット関数
******************************************************************************/
int GetScoreItemCount(void)
{
	return getItemCount;
}

/******************************************************************************
//関数名	：int SetScoreItemCount(void)
//引数1		：int value	：加算値
//戻り値	：void
//説明		：取得したスコアアイテムの数のset関数
******************************************************************************/
void SetScoreItemCount(int value)
{
	getItemCount += value;

	if (scaleState == DEFAULT)
	{
		scaleState = ENLARGEMENT;
	}
}

/******************************************************************************
//関数名	：int GetScoreItemScaleState(void)
//引数1		：void
//戻り値	：int scaleState	：現在のスケールステート
//説明		：スコアアイテムのスケールステート取得関数
******************************************************************************/
float GetScoreItemScale(void)
{
	return scale;
}

#ifdef _DEBUG
/******************************************************************************
//関数名	：int GetScoreItemCount(void)
//引数1		：void
//戻り値	：int		：アクティブなスコアアイテムの数
//説明		：スコアアイテムのアクティブ数取得関数
******************************************************************************/
int GetActiveItemCount(void)
{
	return cntScoreItem;
}

/******************************************************************************
//関数名	：void AddScoreItemCount(int value)
//引数1		：int value	：加算する値
//戻り値	：void
//説明		：スコアアイテムのアクティブ数操作関数
******************************************************************************/
void SetActiveItemCount(int value)
{
	cntScoreItem += value;
}
#endif
