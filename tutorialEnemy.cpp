//=============================================================================
//
// チュートリアルエネミー処理 [tutorialEnemy.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "tutorialEnemy.h"
#include "enemyManager.h"
#include "enemyBullet.h"
#include "particle.h"
#include "sePlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TUTORIALENEMY_TARGETPOS_Y			(400)		//目標座標Y
#define TUTORIALENEMY_MOVESPEED				(8.0f)		//移動スピード

#define TUTORIALENEMY_INITPOS1_X			(450.0f)	//チュートリアルエネミーの初期X座標
#define TUTORIALENEMY_INITPOS2_X			(830.0f)
#define TUTORIALENEMY_INITPOS_Y				(-100.0f)	//チュートリアルエネミーの初期Y座標

#define TUTORIALENEMY_BULLETSPEED			(5.0f)		//バレットのスピード

#define TUTORIALENEMY_SHOTPOS_R				(D3DXVECTOR3(30.0f, 50.0f, 0.0f))	//バレット発射位置右
#define TUTORIALENEMY_SHOTPOS_L				(D3DXVECTOR3(-30.0f, 50.0f, 0.0f))	//バレット発射位置右
#define TUTORIALENEMY_SHOTINTERBAL			(10)		//バレット発射間隔
#define TUTORIALENEMY_INITHP				(200)		//チュートリアルエネミー初期HP

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTutorialEnemy(void);				//頂点作成関数
void SetTextureTutorialEnemy(int cntPattern);		// テクスチャ座標の計算処理
void SetVertexTutorialEnemy(TUTORIALENEMY *ptr);	// 頂点の計算処理
void AttackTutorialEnemy(TUTORIALENEMY *ptr);		// 攻撃処理
void ResetTutorialEnemy(TUTORIALENEMY *ptr);		// チュートリアルエネミーリセット処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum TUTORIALENEMY_STATE
{
	TUTORIALENEMY_MOVE,
	TUTORIALENEMY_ATTACK
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float angle, radius;								//ポリゴンの半径、角度

TUTORIALENEMY tutorialEnemy[TUTORIALENEMY_MAX];			//チュートリアルエネミー配列

//バレットパラメータ（オレンジ）
static ENEMYATTACK bullet1 = {
	ORANGE_CIRCLE,
	VECTOR3_ZERO,
	TUTORIALENEMY_BULLETSPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

//バレットパラメータ（グリーン）
static ENEMYATTACK bullet2 = {
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	TUTORIALENEMY_BULLETSPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTutorialEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIALENEMY* ptr = GetTutorialEnemyAdr(0);
	int i;


	angle = atan2f(TUTORIALENEMY_TEXTURE_SIZE_Y, TUTORIALENEMY_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TUTORIALENEMY_TEXTURE_SIZE_X, TUTORIALENEMY_TEXTURE_SIZE_Y));

	for (i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ptr->pos.x = (i == 0) ? TUTORIALENEMY_INITPOS1_X : TUTORIALENEMY_INITPOS2_X;
		ptr->pos.y = TUTORIALENEMY_INITPOS_Y;

		ptr->targetPos = D3DXVECTOR3(ptr->pos.x, TUTORIALENEMY_TARGETPOS_Y, 0.0f);

		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->state = TUTORIALENEMY_MOVE;
		ptr->id = i;
		ptr->hp = TUTORIALENEMY_INITHP;

		ptr->active = true;
	}

	// 頂点情報の作成
	MakeVertexTutorialEnemy();

	if(num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TUTORIALENEMY_TEXTURE_NAME, pDevice);
	}
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTutorialEnemy(void)
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
void UpdateTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//撃墜処理
		if (ptr->hp <= 0)
		{
			//SetParticle(ptr->pos, EXPLOSION);
			//SetParticle(ptr->pos, PIECE);
			ExplodeMiddleEnemy(ptr->pos);
			PlaySE(SE_MIDDLEEXP);
			ResetTutorialEnemy(ptr);
		}

		if (ptr->state == TUTORIALENEMY_MOVE)
		{
			bool isMoveCompleted = MoveToTarget(&ptr->pos, &ptr->targetPos, TUTORIALENEMY_MOVESPEED);
			if (isMoveCompleted)
			{
				ptr->state = TUTORIALENEMY_ATTACK;
			}
		}

		else if (ptr->state == TUTORIALENEMY_ATTACK)
		{
			ptr->countAnim++;
			if (ptr->countAnim % TUTORIALENEMY_SHOTINTERBAL == 0)
			{
				AttackTutorialEnemy(ptr);
			}
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTutorialEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIALENEMY *ptr = GetTutorialEnemyAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//チュートリアルエネミーを加算合成で描画
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexTutorialEnemy(ptr);

			// テクスチャ座標を設定
			SetTextureTutorialEnemy(ptr->patternAnim);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(TUTORIALENEMY_BB_LEFT, TUTORIALENEMY_BB_TOP, 0.0f), TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
#endif
		}
	}
	//描画方法を通常の方法に再設定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTutorialEnemy(void)
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
void SetTextureTutorialEnemy(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TUTORIALENEMY_TEXTURE_DIVIDE_X;
	int y = cntPattern / TUTORIALENEMY_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TUTORIALENEMY_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TUTORIALENEMY_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTutorialEnemy(TUTORIALENEMY *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;
}

/******************************************************************************
//関数名	：TUTORIALENEMY *GetTutorialEnemyAdr(int pno)
//引数		：int pno：取得したいチュートリアルエネミーのインデックス
//戻り値	：正常な引数：チュートリアルエネミーのアドレス	異常な引数：NULL
//説明		：チュートリアルエネミーのアドレス取得関数
******************************************************************************/
TUTORIALENEMY *GetTutorialEnemyAdr(int pno)
{
	if (pno < TUTORIALENEMY_MAX)
	{
		return &tutorialEnemy[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：TUTORIALENEMY *SetTutorialEnemy(void)
//引数		：チュートリアルエネミーをセットする関数
//戻り値	：void
//説明		：チュートリアルエネミーセット関数
******************************************************************************/
void SetTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ResetTutorialEnemy(ptr);
	}
}

/******************************************************************************
//関数名	：void AttackTutorialEnemy(TUTORIALENEMY *ptr)
//引数		：TUTORIALENEMY *ptr	：攻撃するチュートリアルエネミーへのポインタ
//戻り値	：void
//説明		：チュートリアルエネミー攻撃処理
******************************************************************************/
void AttackTutorialEnemy(TUTORIALENEMY *ptr)
{
	ENEMYATTACK param = (ptr->id % 2 == 0) ? bullet1 : bullet2;

	//右の発射位置から攻撃
	param.pos = ptr->pos + TUTORIALENEMY_SHOTPOS_R;
	SetEnemyBullet(&param, &ptr->hp);

	//左の発射位置から攻撃
	param.pos = ptr->pos + TUTORIALENEMY_SHOTPOS_L;
	SetEnemyBullet(&param, &ptr->hp);
}

/******************************************************************************
//関数名	：void ResetTutorialEnemy(TUTORIALENEMY *ptr)
//引数		：TUTORIALENEMY *ptr	：攻撃するチュートリアルエネミーへのポインタ
//戻り値	：void
//説明		：チュートリアルエネミーリセット処理
******************************************************************************/
void ResetTutorialEnemy(TUTORIALENEMY *ptr)
{
	ptr->pos.x = (ptr->id % 2 == 0) ? TUTORIALENEMY_INITPOS1_X : TUTORIALENEMY_INITPOS2_X;
	ptr->pos.y = TUTORIALENEMY_INITPOS_Y;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;

	ptr->state = TUTORIALENEMY_MOVE;
	ptr->hp = TUTORIALENEMY_INITHP;
	ptr->state = TUTORIALENEMY_MOVE;

	ptr->active = true;
}

/******************************************************************************
//関数名	：void DisableTutorialEnemy(void)
//引数		：void
//戻り値	：void
//説明		：すべてのチュートリアルエネミーを非アクティブにする関数
******************************************************************************/
void DisableTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}