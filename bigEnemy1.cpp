//=============================================================================
//
// ビッグエネミー1処理 [bigEnemy1.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "enemyManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BIGENEMY1_SPEED				(8.0f)					//ビッグエネミー1の移動スピード		
#define BIGENEMY1_ESCAPESPEED		(3.0f)					//ビッグエネミー1の逃走スピード
#define BIGENEMY1_ATTACKINTERBAL	(90)					//ビッグエネミー1の攻撃間隔

#define BIGENEMY1_ATTACK_ANGLE		(7.0f)					//発射するWAY弾のWAYの隙間
#define BIGENEMY1_ATTACK_WAYEASY	(4)						//発射するWAY弾のWAY数(EASY)
#define BIGENEMY1_ATTACK_WAYNORMAL	(6)						//発射するWAY弾のWAY数(NORMAL)
#define BIGENEMY1_ATTACK_WAYHARD	(10)					//発射するWAY弾のWAY数(HARD)
#define BIGENEMY1_ROTATEANGLE		(20.0f)					//発射位置を回転させる角度
#define BIGENEMY1_BULLET_SPEED		(4.5f)					//バレットのスピード
#define BIGENEMY1_ATTACK_OFFSETPOS	(-60.0f)				//バレット発射位置のオフセット座標（Y)

#define BIGENEMY1_WAITFRAME1		(30)					//攻撃開始までの待機フレーム
#define BIGENEMY1_ATTACKFRAME		(BIGENEMY1_ATTACKINTERBAL*6)	//攻撃が終わるまでのフレーム数
#define BIGENEMY1_WAITFRAME2		(240)					//攻撃後の移動開始までの待機フレーム

#define BIGENEMY1_INITHP			(900)					//HP初期値
#define BIGENEMY1_SCORE				(5000)					//撃墜時に加算スコア

#define BIGENEMY1_GAUGE_OFFSETPOS	(D3DXVECTOR3(0.0f, -100.0f, 0.0f))	//HPゲージを表示するオフセット座標
#define BIGENEMY1_SHADOW_OFFSETPOS	(D3DXVECTOR3(100.0f, 200.0f, 0.0f))	//影を表示するオフセット座標

#define BIGENEMY1_CIRCLESHOT_WAY	(18)					//円形弾のWAY数
#define BIGENEMY1_CIRCLESHOT_SPEED	(6.0f)					//円形弾のスピード
#define BIGENEMY1_CIRCLESHOT_DELTA	(0.5f)					//円形弾のスピード増加量
#define BIGENEMY1_CIRCLESHOT_LOOPMAX	(5)					//円形弾のループ回数
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexBigEnemy1(void);							//頂点作成関数
void SetTextureBigEnemy1(int cntPattern, BIGENEMY1 *ptr);	// テクスチャ座標の計算処理
void SetVertexBigEnemy1(BIGENEMY1 *ptr);					// 頂点の計算処理
void SetShadowBigEnemy1(BIGENEMY1 *ptr);					//影の頂点設定処理
void BigEnemy1Action(BIGENEMY1 *ptr);						//行動処理
void BigEnemy1Attack(BIGENEMY1 *ptr);						//攻撃処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum
{
	BIGENEMY1_MOVE,
	BIGENEMY1_WAIT1,
	BIGENEMY1_ATTACK,
	BIGENEMY1_WAIT2,
	BIGENEMY1_ESCAPE
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[BIGENEMY1_TEXTURE_NUM];				// テクスチャへのポインタ

BIGENEMY1 bigEnemy1[BIGENEMY1_MAX];							//ビッグエネミー1配列
static int atkWay;											//バレットのWAY数
static VERTEX_2D		vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

static ENEMYATTACK bulletParam1 =							//攻撃用パラメータ1
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

static ENEMYATTACK bulletParam2 =							//攻撃用パラメータ2
{
	ORANGE_ROTATE,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE + RADIAN(BIGENEMY1_ROTATEANGLE / 2)
};

static ENEMYATTACK circleParam =
{
	ORANGE_WHEEL,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBigEnemy1(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BIGENEMY1* ptr = GetBigEnemy1Adr(0);
	int i;

	//難易度によってWAY数を分岐
	int difficulty = GetDifficulty();
	atkWay = (difficulty == DIFFICULTY_EASY) ? BIGENEMY1_ATTACK_WAYEASY : (difficulty == DIFFICULTY_NORMAL) ? BIGENEMY1_ATTACK_WAYNORMAL : BIGENEMY1_ATTACK_WAYHARD;

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(BIGENEMY1_INITPOS_X, BIGENEMY1_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(BIGENEMY1_TEXTURE_SIZE_Y, BIGENEMY1_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(BIGENEMY1_TEXTURE_SIZE_X, BIGENEMY1_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->targetPos = VECTOR3_ZERO;

		ptr->state = BIGENEMY1_MOVE;

		ptr->hp = BIGENEMY1_INITHP;

		ptr->gauge = NULL;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexBigEnemy1();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[0] = CreateTextureFromFile((LPSTR)BIGENEMY1_SHADOWTEX_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)BIGENEMY1_TEXTURE_NAME, pDevice);
	}
	//SetBigEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_CENTER_X, 100.0f, 0.0));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBigEnemy1(void)
{
	for (int i = 0; i < BIGENEMY1_TEXTURE_NUM; i++)
	{
		if (texture != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateBigEnemy1(void)
{
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);
	ENEMYATTACK param1 = bulletParam1, param2 = bulletParam2;
	int i;

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		//非アクティブなら処理を行わず次のエネミーへ
		if (!ptr->active)
		{
			continue;
		}

		//撃墜判定
		if (ptr->hp <= 0)
		{
			//撃墜処理
			ptr->active = false;
			ExplodeBigEnemy(ptr->pos);
			ConvertBulletToScoreItem();
			AddScore(BIGENEMY1_SCORE);

			//ゲージ解放処理
			ptr->gauge->active = false;
			ptr->gauge = NULL;

			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % TIMEBIGENEMY1_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_BIGENEMY1_NUM;
		}

		//行動処理
		BigEnemy1Action(ptr);

		//ゲージ更新処理
		ptr->gauge->pos = ptr->pos + BIGENEMY1_GAUGE_OFFSETPOS;
		ptr->gauge->scale = (float)ptr->hp / BIGENEMY1_INITHP;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBigEnemy1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture[0]);

	//影を描画
	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点を設定
			SetShadowBigEnemy1(ptr);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	ptr = GetBigEnemy1Adr(0);
	//ビッグエネミー1を描画
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexBigEnemy1(ptr);

			// テクスチャ座標を設定
			SetTextureBigEnemy1(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(BIGENEMY1_BB_LEFT, BIGENEMY1_BB_TOP, 0.0f), BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexBigEnemy1(void)
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
void SetTextureBigEnemy1(int cntPattern, BIGENEMY1 *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % BIGENEMY1_TEXTURE_DIVIDE_X;
	int y = cntPattern / BIGENEMY1_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BIGENEMY1_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BIGENEMY1_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexBigEnemy1(BIGENEMY1 *ptr)
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
影用の頂点設定関数
******************************************************************************/
void SetShadowBigEnemy1(BIGENEMY1 *ptr)
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

	//影用に頂点座標をずらす
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += BIGENEMY1_SHADOW_OFFSETPOS;
	}

	//テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
/******************************************************************************
//関数名	：BIGENEMY1 *GetBigEnemy1Adr(int pno)
//引数		：int pno：取得したいビッグエネミー1のインデックス
//戻り値	：正常な引数：ビッグエネミー1のアドレス	異常な引数：NULL
//説明		：ビッグエネミー1のアドレス取得関数
******************************************************************************/
BIGENEMY1 *GetBigEnemy1Adr(int pno)
{
	if (pno < BIGENEMY1_MAX)
	{
		return &bigEnemy1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void BigEnemy1Action(BIGENEMY1 *ptr)
//引数		：BIGENEMY1 *ptr		：行動するビッグエネミー1へのポインタ
//戻り値	：void
//説明		：ビッグエネミー1の行動処理
******************************************************************************/
void BigEnemy1Action(BIGENEMY1 *ptr)
{
	bool flgMove = false;

	switch (ptr->state)
	{
		//目標座標まで移動
	case BIGENEMY1_MOVE:
		flgMove = MoveToTarget(&ptr->pos, &ptr->targetPos, BIGENEMY1_SPEED);
		//目標座標へ到達していたら次の状態へ遷移
		if (flgMove)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//攻撃開始までの待機
	case BIGENEMY1_WAIT1:
		ptr->cntFrame++;
		if (ptr->cntFrame >= BIGENEMY1_WAITFRAME1)
		{
			ptr->cntFrame = 0;
			BigEnemy1Attack(ptr);
			ptr->state++;
		}
		break;

		//攻撃
	case BIGENEMY1_ATTACK:
		ptr->cntFrame++;
		//一定間隔で攻撃
		if (ptr->cntFrame % BIGENEMY1_ATTACKINTERBAL == 0)
		{
			BigEnemy1Attack(ptr);
		}

		//一定フレーム経過したら次の状態へ遷移
		if (ptr->cntFrame >= BIGENEMY1_ATTACKFRAME)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//攻撃後の待機
	case BIGENEMY1_WAIT2:
		ptr->cntFrame++;
		if (ptr->cntFrame >= BIGENEMY1_WAITFRAME2)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//画面下へ徐々に移動
	case BIGENEMY1_ESCAPE:
		ptr->cntFrame++;
		StartAccel(&ptr->pos, &(VECTOR3_UP * -1), BIGENEMY1_ESCAPESPEED, 60, ptr->cntFrame);

		//画面外へ出たら非アクティブに
		if (ptr->pos.y > SCREEN_HEIGHT + BIGENEMY1_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//関数名	：void BigEnemy1Attack(void)
//引数		：BIGENEMY1 *ptr	：攻撃をおこなるビッグエネミー1へのポインタ
//戻り値	：void
//説明		：ビッグエネミー1の攻撃
******************************************************************************/
void BigEnemy1Attack(BIGENEMY1 *ptr)
{
	ENEMYATTACK param1 = bulletParam1, param2 = bulletParam2;
	float angle;

	//発射地点を現在座標に設定
	param1.pos = ptr->pos;
	param2.pos = ptr->pos;
	param1.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;
	param2.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;

	//発射角度が一周するまでループ
	for (angle = 0.0f; angle < 360.0f; angle += BIGENEMY1_ROTATEANGLE)
	{
		//バレットを発射
		FireWayBullet(&param1, atkWay, (float)atkWay - 1, &ptr->hp);
		FireWayBullet(&param2, atkWay, (float)atkWay - 1, &ptr->hp);

		//発射角度を回転
		param1.angle += RADIAN(BIGENEMY1_ROTATEANGLE);
		param2.angle += RADIAN(BIGENEMY1_ROTATEANGLE);
	}

	//難易度HARDなら円形弾追加
	if (GetDifficulty() == DIFFICULTY_HARD)
	{
		ENEMYATTACK param3 = circleParam;
		PLAYER *player = GetPlayerAdr(0);
		
		param3.pos = ptr->pos;
		param3.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;
		param3.angle = SnipeTarget(&param3.pos, &player->pos);

		for (int i = 0; i < BIGENEMY1_CIRCLESHOT_LOOPMAX; i++)
		{
			FireCircleBullet(&param3, BIGENEMY1_CIRCLESHOT_WAY, &ptr->hp);
			param3.speed += BIGENEMY1_CIRCLESHOT_DELTA;
		}
	}
}

/******************************************************************************
//関数名	：void SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//引数1		：D3DXVECTOR3 initPos		：出現座標
//引数2		：D3DXVECTOR3 targetPos		：目標座標
//戻り値	：void
//説明		：ビッグエネミー1をゲーム内に出現させる関数
******************************************************************************/
void SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i = 0;
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			//各座標を設定
			ptr->pos = initPos;
			ptr->targetPos = targetPos;

			//各パラメータを初期化
			ptr->gauge = GetNonActiveEnemyGauge();
			ptr->hp = BIGENEMY1_INITHP;
			ptr->state = BIGENEMY1_MOVE;
			ptr->patternAnim = 0;
			ptr->countAnim = 0;

			//エネミーをアクティブにセットしリターン
			ptr->state = BIGENEMY1_MOVE;
			ptr->active = true;
			return;
		}
	}
}