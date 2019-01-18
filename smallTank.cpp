//=============================================================================
//
// 処理 [middleTank.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bg.h"
#include "enemyManager.h"
#include "groundFire.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SMALLTANK_INITHP					(30)				//体力初期値
#define SMALLTANK_SCORE						(500)				//撃墜時のスコア
#define SMALLTANK_BULLET_SPEED				(5.5f)				//バレットの速度
#define SMALLTANK_MOVE_SPEED				(2.5f)				//移動速度
#define SMALLTANK_SCROLLSPEED				(-BG_STAGE1_SPEED3*SCREEN_HEIGHT) //背景に合わせたスクロール速度

#define SMALLTANK_TURRET_ROTANGLE			(RADIAN(10.0f))		//砲塔の回転角度

#define SMALLTANK_INITFRAMEEASY					(240)			//INIT状態で待機するフレーム数
#define SMALLTANK_INITFRAMENORMAL				(120)			//INIT状態で待機するフレーム数
#define SMALLTANK_INITFRAMEHARD					(30)			//INIT状態で待機するフレーム数

#define SMALLTANK_ATTACK_MAX				(4)					//最大攻撃回数
#define SMALLTANK_ATTACK_INTERBAL			(5)					//攻撃間隔
#define SMALLTANK_ATTACK_FRAME				(SMALLTANK_ATTACK_INTERBAL*3) //攻撃にかける時間

#define SMALLTANK_WAIT_FRAME				(90)				//待機時間

#define SMALLTANK_SHOTPOS_X					(0.0f)				//バレット発射位置のX座標
#define SMALLTANK_SHOTPOS_Y					(50.0f)				//バレット発射位置のY座標

#define SMALLTANK_ATTACK_WAY				(3)					//WAY弾のWAY数
#define SMALLTANK_ATTACK_RANGE				(60)				//WAY弾の角度

#define SMALLTANK_SETTER_INTERBAL			(30)				//セッターがタンクを出現させる間隔
#define SMALLTANK_SETTER_SETMAX				(8)				//セッターがタンクを出現させる最大数

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexSmallTank(void);								//頂点作成関数
void SetTextureSmallTank(int cntPattern, SMALLTANK *ptr);		// テクスチャ座標の計算処理
void SetVertexSmallTank(SMALLTANK *ptr);						// 頂点の計算処理
void SetTurretVertex(SMALLTANK *ptr);							//頂点の計算処理（砲塔用)
void ActionSmallTank(SMALLTANK *ptr);							//行動処理
void AttackSmallTank(SMALLTANK *ptr);							//攻撃処理

/*****************************************************************************
構造体定義
*****************************************************************************/
//テクスチャ番号の列挙体
enum
{
	BODY,
	TURRET
};

//行動状態の列挙体
enum
{
	INIT,
	ATTACK,
	WAIT,
	ESCAPE
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SMALLTANK_TEXTURE_NUM];			// テクスチャへのポインタ

SMALLTANK middleTank[SMALLTANK_MAX];									//ミドルタンク配列

static VERTEX_2D		vertexWk[NUM_VERTEX];							//頂点情報格納ワーク
static SMALLTANK_SETTER setter[SMALLTANK_SETTER_MAX];					//セッター配列
static int initWaitFrame;												//INIT状態から遷移する時間

//攻撃用パラメータ
static ENEMYATTACK bulletParam1 =
{
	ORANGE_CIRCLE,
	VECTOR3_ZERO,
	SMALLTANK_BULLET_SPEED,
	0.0f
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSmallTank(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLTANK* ptr = GetSmallTankAdr(0);
	int i;

	//INITでの待機時間を難易度で分岐
	int difficulty = GetDifficulty();
	initWaitFrame = (difficulty == DIFFICULTY_EASY) ? SMALLTANK_INITFRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLTANK_INITFRAMENORMAL : SMALLTANK_INITFRAMEHARD;

	//タンクの初期化
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLTANK_INITPOS_X, SMALLTANK_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SMALLTANK_TEXTURE_SIZE_Y, SMALLTANK_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SMALLTANK_TEXTURE_SIZE_X, SMALLTANK_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->turretRot = 0.0f;
		ptr->countAnim = 0;
		ptr->moveDir = VECTOR3_ZERO;
		ptr->state = INIT;

		ptr->hp = SMALLTANK_INITHP;
		ptr->attackNum = 0;

		ptr->active = false;
	}

	//セッターの初期化
	for (i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		setter[i].active = false;
		setter[i].cntFrame = 0;
		setter[i].cntSet = 0;
		setter[i].pos = VECTOR3_ZERO;
		setter[i].targetPos = VECTOR3_ZERO;
	}

	// 頂点情報の作成
	MakeVertexSmallTank();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLTANK_TEXTURE_NAME, pDevice);
		texture[TURRET] = CreateTextureFromFile((LPSTR)SMALLTANK_TURRETTEX_NAME, pDevice);
	}
	//SetSmallTank(D3DXVECTOR3(BG_LEFT, 0.0f, 0.0f), D3DXVECTOR3(BG_RIGHT, 1.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSmallTank(void)
{
	for (int i = 0; i < SMALLTANK_TEXTURE_NUM; i++)
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
void UpdateSmallTank(void)
{
	int i;
	SMALLTANK *ptr = GetSmallTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	//セッターの更新
	for (i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		if (setter[i].active)
		{
			setter[i].pos.y += SMALLTANK_SCROLLSPEED;
			setter[i].targetPos.y += SMALLTANK_SCROLLSPEED;
			if (setter[i].cntFrame % SMALLTANK_SETTER_INTERBAL == 0)
			{
				SetSmallTank(setter[i].pos, setter[i].targetPos);
				setter[i].cntSet++;

				if (setter[i].cntSet >= SMALLTANK_SETTER_SETMAX)
				{
					setter[i].active = false;
				}
			}

			setter[i].cntFrame++;
		}
	}

	//タンクの更新
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		//非アクティブなら次のエネミーへ
		if (!ptr->active)
		{
			continue;
		}

		//撃墜判定
		if (ptr->hp <= 0)
		{
			//撃墜処理
			ptr->active = false;
			AddScore(SMALLTANK_SCORE);
			ExplodeSmallEnemy(ptr->pos);
			SetGroundFire(ptr->pos, ptr->rot);
			continue;
		}

		//座標の更新
		ptr->pos += ptr->moveDir * SMALLTANK_MOVE_SPEED;
		ptr->pos.y += SMALLTANK_SCROLLSPEED;

		//座標が画面外であれば非アクティブへ
		if (ptr->state != INIT)
		{
			if (ptr->pos.x + SMALLTANK_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - SMALLTANK_TEXTURE_SIZE_X > BG_RIGHT
				|| ptr->pos.y + SMALLTANK_TEXTURE_SIZE_Y < 0.0f || ptr->pos.y - SMALLTANK_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
			{
				ptr->active = false;
			}
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % SMALLTANK_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLTANK_ANIM_NUM;
		}

		//砲塔を回転
		if (ptr->state != ATTACK)
		{
			ptr->turretRot += RotateToTarget(&ptr->pos, &player->pos, ptr->turretRot, SMALLTANK_TURRET_ROTANGLE);
		}

		//行動処理
		ActionSmallTank(ptr);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawSmallTank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLTANK *ptr = GetSmallTankAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ミドルタンクの本体を描画
	pDevice->SetTexture(0, texture[BODY]);
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexSmallTank(ptr);

			// テクスチャ座標を設定
			SetTextureSmallTank(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//ミドルタンクの砲塔を描画
	ptr = GetSmallTankAdr(0);
	pDevice->SetTexture(0, texture[TURRET]);
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetTurretVertex(ptr);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLTANK_BB_LEFT, SMALLTANK_BB_TOP, 0.0f), SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexSmallTank(void)
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
void SetTextureSmallTank(int cntPattern, SMALLTANK *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % SMALLTANK_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLTANK_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLTANK_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLTANK_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定(本体用)
******************************************************************************/
void SetVertexSmallTank(SMALLTANK *ptr)
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
頂点座標の設定(砲塔用)
******************************************************************************/
void SetTurretVertex(SMALLTANK *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->turretRot) * ptr->radius;
}

/******************************************************************************
//関数名	：SMALLTANK *GetSmallTankAdr(int pno)
//引数		：int pno：取得したいミドルタンクのインデックス
//戻り値	：正常な引数：ミドルタンクのアドレス	異常な引数：NULL
//説明		：ミドルタンクのアドレス取得関数（メインショット）
******************************************************************************/
SMALLTANK *GetSmallTankAdr(int pno)
{
	if (pno < SMALLTANK_MAX)
	{
		return &middleTank[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//引数1		：D3DXVECTOR3 initPos			：出現座標
//引数2		：D3DXVECTOR3 targetPos			：目標座標
//戻り値	：void
//説明		：ミドルタンクのセット関数
******************************************************************************/
void SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i;
	SMALLTANK *ptr = GetSmallTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			//各座標の設定
			ptr->pos = initPos;
			D3DXVECTOR3 tmp = targetPos - initPos;
			D3DXVec3Normalize(&ptr->moveDir, &tmp);

			//ポリゴンの回転量を設定
			tmp = ptr->pos + ptr->moveDir;
			ptr->rot.z = SnipeTarget(&ptr->pos, &tmp) - RADIAN(90);;

			//各パラメータを初期化
			ptr->turretRot = SnipeTarget(&ptr->pos, &player->pos) - RADIAN(90);
			ptr->countAnim = 0;
			ptr->state = INIT;
			ptr->hp = SMALLTANK_INITHP;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->attackNum = 0;

			//ミドルタンクを出現させリターン
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//関数名	：void ActionSmallTank(SMALLTANK *ptr)
//引数1		：SMALLTANK *ptr		：行動を行うミドルタンクへのポインタ
//戻り値	：void
//説明		：ミドルタンクの行動処理
******************************************************************************/
void ActionSmallTank(SMALLTANK *ptr)
{
	ptr->countFrame++;

	switch (ptr->state)
	{
		//出現直後の状態
	case INIT:
		//出現してから一定時間経過したらATTACK状態へ遷移
		if (ptr->countFrame >= initWaitFrame)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//攻撃状態
	case ATTACK:
		if (ptr->countFrame % SMALLTANK_ATTACK_INTERBAL == 0)
		{
			//攻撃処理
			AttackSmallTank(ptr);
		}

		//攻撃回数が最大へ到達していたら状態遷移
		if (ptr->attackNum >= SMALLTANK_ATTACK_MAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//攻撃時間が終わっていれば待機状態へ遷移
		else if (ptr->countFrame >= SMALLTANK_ATTACK_FRAME)
		{
			ptr->countFrame = 0;
			ptr->attackNum++;
			ptr->state = WAIT;
		}
		break;

		//待機状態
	case WAIT:
		//一定時間経過していたら攻撃状態へ遷移
		if (ptr->countFrame > SMALLTANK_WAIT_FRAME)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

	case ESCAPE:
		//この状態の時は移動以外何もしない
		break;
	}
}

/******************************************************************************
//関数名	：void AttackSmallTank(SMALLTANK *ptr)
//引数1		：SMALLTANK *ptr		：攻撃を行うミドルタンクへのポインタ
//戻り値	：void
//説明		：ミドルタンクの攻撃処理
******************************************************************************/
void AttackSmallTank(SMALLTANK *ptr)
{
	ENEMYATTACK param = bulletParam1;
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	float shotAngle = ptr->turretRot + RADIAN(90);

	//発射位置の計算
	shotPos.x = ptr->pos.x + cosf(shotAngle) * SMALLTANK_SHOTPOS_Y;
	shotPos.y = ptr->pos.y + sinf(shotAngle) * SMALLTANK_SHOTPOS_Y;

	//パラメータを設定しバレットを発射
	param.pos = shotPos;
	param.angle = shotAngle;
	SetEnemyBullet(&param, &ptr->hp);
}

/******************************************************************************
//関数名	：void CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
//引数1		：D3DXVECTOR3 initPos			：出現座標
//引数2		：D3DXVECTOR3 targetPos			：目標座標
//戻り値	：void
//説明		：ミドルタンクのセッター設定関数
******************************************************************************/
void CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
{
	for (int i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		if (!setter[i].active)
		{
			setter[i].active = true;
			setter[i].cntFrame = 0;
			setter[i].cntSet = 0;
			setter[i].pos = initPos;
			setter[i].targetPos = target;
			return;
		}
	}
}