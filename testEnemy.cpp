//=============================================================================
//
// テストエネミー処理 [testEnemy.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "testEnemy.h"
#include "player.h"
#include "myLibrary.h"
#include "effect.h"
#include "particle.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
enum
{
	MOVE_TARGET,
	ATTACK,
	ESCAPE
};

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTestEnemy(TESTENEMY *ptr);				//頂点作成関数
void SetTextureTestEnemy(int cntPattern, TESTENEMY *ptr);	// テクスチャ座標の計算処理
void SetVertexTestEnemy(TESTENEMY *ptr);					// 頂点の計算処理

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;						// テクスチャへのポインタ（メイン用）

TESTENEMY				testEnemy[TESTENEMY_MAX];				//プレイヤーバレット配列(メインショット)

static VERTEX_2D		vertexWk[NUM_VERTEX];						//頂点情報格納ワーク

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTestEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TESTENEMY* ptr = GetTestEnemyAdr(0);
	int i;

	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(TESTENEMY_INITPOS_X, TESTENEMY_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(TESTENEMY_TEXTURE_SIZE_Y, TESTENEMY_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TESTENEMY_TEXTURE_SIZE_X, TESTENEMY_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->state = 0;
		ptr->active = false;

		// 頂点情報の作成
		MakeVertexTestEnemy(ptr);
	}

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TESTENEMY_TEXTURE_NAME, pDevice);
	}

	ptr = GetTestEnemyAdr(0);
	ptr->pos = D3DXVECTOR3(-50.0f, -50.0f, 0.0f);
	//ptr->active = true;

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTestEnemy(void)
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
void UpdateTestEnemy(void)
{
	TESTENEMY *ptr = GetTestEnemyAdr(0);
	PLAYER *player = GetPlayerAdr(0);
	ENEMYATTACK param = { ORANGE_CIRCLE, ptr->pos, 5.0f, ENEMYBULLET_DAFAULT_ANGLE };
	int i;

	//更新
	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//撃墜判定
			if (ptr->hp <= 0)
			{
				ptr->active = false;
			}

			ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, RADIAN(10.0f));
			switch (ptr->state)
			{
			case MOVE_TARGET:
				if (MoveToTarget(&ptr->pos, &D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 10.0f))
				{
					ptr->state = ATTACK;
				}
				break;
			case ATTACK:
				ptr->interbal++;
				if (ptr->interbal % 10 == 0)
				{
					param.angle = ENEMYBULLET_DAFAULT_ANGLE + ptr->rot.z;
					SetEnemyBullet(&param, &ptr->hp);
					ptr->attackNum++;
					if (ptr->attackNum > 10)
					{
						ptr->state = ESCAPE;
						ptr->interbal = 0;
					}
				}
				break;
			case ESCAPE:
				ptr->interbal++;
				StartAccel(&ptr->pos, &VECTOR3_UP, 8.0f, 60, ptr->interbal);
				break;
			}
			//ptr->rot.z += RotateToTarget(&(ptr->pos), &(player->pos), ptr->rot.z, RADIAN(5.0f));
		}
	}
}


/******************************************************************************
描画処理
******************************************************************************/
void DrawTestEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TESTENEMY *ptr = GetTestEnemyAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//プレイヤーバレットを描画
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexTestEnemy(ptr);

			// テクスチャ座標を設定
			SetTextureTestEnemy(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(TESTENEMY_BB_LEFT, TESTENEMY_BB_TOP, 0.0f), TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTestEnemy(TESTENEMY *ptr)
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
void SetTextureTestEnemy(int cntPattern, TESTENEMY *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % TESTENEMY_TEXTURE_DIVIDE_X;
	int y = cntPattern / TESTENEMY_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TESTENEMY_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TESTENEMY_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTestEnemy(TESTENEMY *ptr)
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
//関数名	：TESTENEMY *GetTestEnemyAdr(int pno)
//引数		：int pno：取得したいプレイヤーバレットのインデックス
//戻り値	：正常な引数：プレイヤーバレットのアドレス	異常な引数：NULL
//説明		：プレイヤーバレットのアドレス取得関数（メインショット）
******************************************************************************/
TESTENEMY *GetTestEnemyAdr(int pno)
{
	if (pno < TESTENEMY_MAX)
	{
		return &testEnemy[pno];
	}
	else
	{
		return NULL;
	}
}