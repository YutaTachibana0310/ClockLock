//=============================================================================
//
// スモールザコ2処理 [smallZako1.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SMALLZAKO2_INITHP					(10)				//HP初期値
#define SMALLZAKO2_SCORE					(100)				//撃墜時のスコア
#define SMALLZAKO2_INITSPEED				(10.0f)				//スピード初期値

#define SMALLZAKO2_INIT_FRAME				(30)				//INIT状態から次へ遷移する時間
#define SMALLZAKO2_INIT_RADIAN(f)			(RADIAN((f/2.0f)*180/SMALLZAKO2_INIT_FRAME))	//INIT状態で減速するための係数

#define SMALLZAKO2_WAIT_FRAME				(30)				//WAIT状態から次へ遷移する時間

#define SMALLZAKO2_ATTACK_VALUE				(0.5f)				//バレットの速度の加算値
#define SMALLZAKO2_ATTACK_NUMEASY			(1)					//一度に撃つバレットの数
#define SMALLZAKO2_ATTACK_NUMNORMAL			(3)					//一度に撃つバレットの数
#define SMALLZAKO2_ATTACK_NUMHARD			(6)					//一度に撃つバレットの数
#define SMALLZAKO2_ATTACK_MAX				(3)					//攻撃最大回数
#define SMALLZAKO2_BULLET_SPEED				(4.5f)				//バレットのスピード

#define SMALLZAKO2_INTERBAL_FRAME			(120)				//INTERBAL状態から遷移する時間

#define SMALLZAKO2_HOMING_ANGLE				(RADIAN(2.0f))		//追尾時の最小回転角度

#define SMALLZAKO2_SHADOW_OFFSETPOS			(D3DXVECTOR3(50.0f, 100.0f, 0.0f)) //影のオフセット座標

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexSmallZako2(void);							//頂点作成関数
void SetTextureSmallZako2(int cntPattern, SMALLZAKO2 *ptr);	// テクスチャ座標の計算処理
void SetVertexSmallZako2(SMALLZAKO2 *ptr);					// 頂点の計算処理
void SetShadowSmallZako2(SMALLZAKO2 *ptr);					//影用の設定処理
void ActionSmallZako2(SMALLZAKO2 *ptr);						//スモールザコ2の行動処理
void AttackSmallZako2(SMALLZAKO2 *ptr);						//スモールザコ2の攻撃処理

/*****************************************************************************
構造体定義
*****************************************************************************/
//テクスチャ番号の列挙体
enum
{
	SHADOW,
	BODY
};

//行動状態列挙体
enum
{
	INIT,
	WAIT,
	ATTACK,
	INTERBAL,
	ESCAPE
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLZAKO2_TEXTURE_NUM];		// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];							//頂点情報格納ワーク

static float angle;												//ポリゴンの半径
static float radius;											//ポリゴンの頂点角度
static int	atkNum;												//攻撃回数

//攻撃時のバレットパラメータ
static ENEMYATTACK bulletParam =
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	SMALLZAKO2_BULLET_SPEED,
	0.0f
};

SMALLZAKO2 smallZako1[SMALLZAKO2_MAX];							//スモールザコ2配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSmallZako2(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO2* ptr = GetSmallZako2Adr(0);
	int i;

	angle = atan2f(SMALLZAKO2_TEXTURE_SIZE_Y, SMALLZAKO2_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SMALLZAKO2_TEXTURE_SIZE_X, SMALLZAKO2_TEXTURE_SIZE_Y));

	//難易度によって攻撃回数を分岐
	int difficulty = GetDifficulty();
	atkNum = (difficulty == DIFFICULTY_EASY) ? SMALLZAKO2_ATTACK_NUMEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLZAKO2_ATTACK_NUMNORMAL : SMALLZAKO2_ATTACK_NUMHARD;

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLZAKO2_INITPOS_X, SMALLZAKO2_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->hp = SMALLZAKO2_INITHP;
		ptr->state = INIT;
		ptr->speed = SMALLZAKO2_INITSPEED;
		ptr->moveDir = VECTOR3_UP * -1;
		ptr->cntFrame = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexSmallZako2();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[SHADOW] = CreateTextureFromFile((LPSTR)SMALLZAKO2_SHADOWTEX_NAME, pDevice);
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLZAKO2_TEXTURE_NAME, pDevice);
	}
	//SetSmallZako2(D3DXVECTOR3(BG_LEFT + 200.0f, 0.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSmallZako2(void)
{
	for (int i = 0; i < SMALLZAKO2_TEXTURE_NUM; i++)
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
void UpdateSmallZako2(void)
{
	int i;
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		//非アクティブであれば処理を行わず次へ
		if (!ptr->active)
		{
			continue;
		}

		//撃墜処理
		if (ptr->hp <= 0)
		{
			ptr->active = false;
			ExplodeSmallEnemy(ptr->pos);
			AddScore(SMALLZAKO2_SCORE);
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % SMALLZAKO2_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLZAKO2_ANIM_NUM;
		}

		//プレイヤーの方へ向く
		ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLZAKO2_HOMING_ANGLE);

		//行動処理
		ActionSmallZako2(ptr);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawSmallZako2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//影を描画
	pDevice->SetTexture(0, texture[SHADOW]);
	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標、テクスチャ座標を設定
			SetShadowSmallZako2(ptr);

			//ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//本体を描画
	pDevice->SetTexture(0, texture[BODY]);
	ptr = GetSmallZako2Adr(0);
	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexSmallZako2(ptr);

			// テクスチャ座標を設定
			SetTextureSmallZako2(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLZAKO2_BB_LEFT, SMALLZAKO2_BB_TOP, 0.0f), SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexSmallZako2(void)
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
void SetTextureSmallZako2(int cntPattern, SMALLZAKO2 *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % SMALLZAKO2_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLZAKO2_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLZAKO2_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLZAKO2_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexSmallZako2(SMALLZAKO2 *ptr)
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
影用の頂点座標、テクスチャ座標の設定
******************************************************************************/
void SetShadowSmallZako2(SMALLZAKO2 *ptr)
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

	//影用にオフセット
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += SMALLZAKO2_SHADOW_OFFSETPOS;
	}

	//テクスチャ座標を設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//関数名	：SMALLZAKO2 *GetSmallZako2Adr(int pno)
//引数		：int pno：取得したいスモールザコ2のインデックス
//戻り値	：正常な引数：スモールザコ2のアドレス	異常な引数：NULL
//説明		：スモールザコ2のアドレス取得関数
******************************************************************************/
SMALLZAKO2 *GetSmallZako2Adr(int pno)
{
	if (pno < SMALLZAKO2_MAX)
	{
		return &smallZako1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetSmallZako2(D3DXVECTOR3 initPos)
//引数		：D3DXVECTOR3 initPos	：出現座標
//戻り値	：void
//説明		：スモールザコ2のセット関数
******************************************************************************/
void SetSmallZako2(D3DXVECTOR3 initPos)
{
	int i;
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		//非アクティブのスモールザコ2がいればセット
		if (!ptr->active)
		{
			//各パラメータを設定
			ptr->pos = initPos;
			ptr->rot.z = 0.0f;
			ptr->hp = SMALLZAKO2_INITHP;
			ptr->moveDir = VECTOR3_UP * -1;
			ptr->speed = SMALLZAKO2_INITSPEED;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->cntFrame = 0;
			ptr->cntAttack = 0;
			ptr->state = INIT;

			//セットしリターン
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//関数名	：void ActionSmallZako2(SMALLZAKO2 *ptr)
//引数		：SMALLZAKO21 *ptr	：行動するエネミーへのポインタ
//戻り値	：void
//説明		：スモールザコ2の行動処理
******************************************************************************/
void ActionSmallZako2(SMALLZAKO2 *ptr)
{
	PLAYER *player = GetPlayerAdr(0);

	ptr->cntFrame++;

	switch (ptr->state)
	{
	case INIT:
		//目標方向へ向かって減速移動
		ptr->speed = cosf(SMALLZAKO2_INIT_RADIAN(ptr->cntFrame)) * SMALLZAKO2_INITSPEED;
		ptr->pos += ptr->moveDir * ptr->speed;
		//一定時間経過していたら次の状態へ遷移
		if (ptr->cntFrame >= SMALLZAKO2_INIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = WAIT;
		}
		break;

	case WAIT:
		//その場で一定時間待機
		if (ptr->cntFrame >= SMALLZAKO2_WAIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = ATTACK;
		}
		break;

	case ATTACK:
		//攻撃し状態遷移
		AttackSmallZako2(ptr);
		ptr->cntFrame = 0;
		ptr->state = INTERBAL;
		break;

	case INTERBAL:
		//一定時間待機
		if (ptr->cntFrame >= SMALLZAKO2_INTERBAL_FRAME)
		{
			//攻撃回数が最大であればESCAPE状態へ遷移
			if (ptr->cntAttack >= SMALLZAKO2_ATTACK_MAX)
			{
				ptr->cntFrame = 0;
				ptr->state = ESCAPE;
			}
			//再び攻撃状態へ遷移
			else
			{
				ptr->cntFrame = 0;
				ptr->state = ATTACK;
			}
		}
		break;

	case ESCAPE:
		//画面上部へ逃走
		StartAccel(&ptr->pos, &VECTOR3_UP, SMALLZAKO2_INITSPEED, SMALLZAKO2_INIT_FRAME, ptr->cntFrame);

		//画面外に出ていれば非アクティブへ
		if (ptr->pos.y + SMALLZAKO2_TEXTURE_SIZE_Y < 0.0f)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//関数名	：void AttackSmallZako2(SMALLZAKO2 *ptr)
//引数		：SMALLZAKO2 *ptr	：攻撃するスモールザコ2へのポインタ
//戻り値	：void
//説明		：スモールザコ2の攻撃処理
******************************************************************************/
void AttackSmallZako2(SMALLZAKO2 *ptr)
{
	ptr->cntAttack++;

	//難易度EASYなら攻撃しない
	if (GetDifficulty() == DIFFICULTY_EASY)
	{
		return;
	}

	ENEMYATTACK param = bulletParam;

	//各パラメータを設定
	param.angle = ptr->rot.z + RADIAN(90);
	param.pos = ptr->pos;

	//攻撃
	for (int i = 0; i < atkNum; i++)
	{
		SetEnemyBullet(&param, &ptr->hp);
		param.speed += SMALLZAKO2_ATTACK_VALUE;
	}
}