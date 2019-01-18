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
#define MIDDLETANK_INITHP					(30)				//体力初期値
#define MIDDLETANK_SCORE					(500)				//撃墜時のスコア
#define MIDDLETANK_BULLET_SPEED				(5.0f)				//バレットの速度
#define MIDDLETANK_MOVE_SPEED				(3.5f)				//移動速度
#define MIDDLETANK_SCROLLSPEED				(-BG_STAGE1_SPEED3*SCREEN_HEIGHT) //背景に合わせたスクロール速度

#define MIDDLETANK_TURRET_ROTANGLE			(RADIAN(10.0f))		//砲塔の回転角度

#define MIDDLETANK_INITFRAME				(120)				//INIT状態で待機するフレーム数

#define MIDDLETANK_ATTACK_MAX				(4)					//最大攻撃回数
#define MIDDLETANK_ATTACK_INTERBAL			(3)					//攻撃間隔
#define MIDDLETANK_ATTACK_FRAMEEASY			(MIDDLETANK_ATTACK_INTERBAL*2) //攻撃にかける時間
#define MIDDLETANK_ATTACK_FRAMENORMAL		(MIDDLETANK_ATTACK_INTERBAL*4) //攻撃にかける時間
#define MIDDLETANK_ATTACK_FRAMEHARD			(MIDDLETANK_ATTACK_INTERBAL*8) //攻撃にかける時間

#define MIDDLETANK_WAIT_FRAME				(90)				//待機時間

#define MIDDLETANK_SHOTPOS_X				(0.0f)				//バレット発射位置のX座標
#define MIDDLETANK_SHOTPOS_Y				(50.0f)				//バレット発射位置のY座標

#define MIDDLETANK_ATTACK_WAYEASY				(2)					//WAY弾のWAY数（EASY)	
#define MIDDLETANK_ATTACK_WAY				(3)					//WAY弾のWAY数
#define MIDDLETANK_ATTACK_RANGE				(60)				//WAY弾の角度

#define MIDDLETANK_SETTER_INTERBAL			(45)				//セッターがタンクを出現させる間隔
#define MIDDLETANK_SETTER_SETMAX			(8)					//セッターがタンクを出現させる最大数
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexMiddleTank(void);								//頂点作成関数
void SetTextureMiddleTank(int cntPattern, MIDDLETANK *ptr);		// テクスチャ座標の計算処理
void SetVertexMiddleTank(MIDDLETANK *ptr);						// 頂点の計算処理
void SetTurretVertex(MIDDLETANK *ptr);							//頂点の計算処理（砲塔用)
void ActionMiddleTank(MIDDLETANK *ptr);							//行動処理
void AttackMiddleTank(MIDDLETANK *ptr);							//攻撃処理

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
static LPDIRECT3DTEXTURE9		texture[MIDDLETANK_TEXTURE_NUM];		// テクスチャへのポインタ

MIDDLETANK middleTank[MIDDLETANK_MAX];									//ミドルタンク配列

static VERTEX_2D		vertexWk[NUM_VERTEX];							//頂点情報格納ワーク
static MIDDLETANK_SETTER setter[MIDDLETANK_SETTER_MAX];					//ミドルタンクセッター

static int atkFrame;													//攻撃時間

//攻撃用パラメータ
static ENEMYATTACK bulletParam1 =
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	MIDDLETANK_BULLET_SPEED,
	0.0f
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitMiddleTank(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MIDDLETANK* ptr = GetMiddleTankAdr(0);
	int i;

	//難易度によって攻撃時間を分岐
	int difficulty = GetDifficulty();
	atkFrame = (difficulty == DIFFICULTY_EASY) ? MIDDLETANK_ATTACK_FRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? MIDDLETANK_ATTACK_FRAMENORMAL : MIDDLETANK_ATTACK_FRAMEHARD;
	//タンクの初期化
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(MIDDLETANK_INITPOS_X, MIDDLETANK_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(MIDDLETANK_TEXTURE_SIZE_Y, MIDDLETANK_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(MIDDLETANK_TEXTURE_SIZE_X, MIDDLETANK_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->turretRot = 0.0f;
		ptr->countAnim = 0;
		ptr->moveDir = VECTOR3_ZERO;
		ptr->state = INIT;

		ptr->hp = MIDDLETANK_INITHP;
		ptr->attackNum = 0;
		ptr->shotWay = (GetDifficulty() == DIFFICULTY_EASY) ? MIDDLETANK_ATTACK_WAYEASY : MIDDLETANK_ATTACK_WAY;

		ptr->active = false;
	}

	//セッターの初期化
	for (i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		setter[i].active = false;
		setter[i].pos = VECTOR3_ZERO;
		setter[i].cntFrame = 0;
		setter[i].cntSet = 0;
	}

	// 頂点情報の作成
	MakeVertexMiddleTank();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[BODY] = CreateTextureFromFile((LPSTR)MIDDLETANK_TEXTURE_NAME, pDevice);
		texture[TURRET] = CreateTextureFromFile((LPSTR)MIDDLETANK_TURRETTEX_NAME, pDevice);
	}
	//SetMiddleTank(D3DXVECTOR3(BG_LEFT, 0.0f, 0.0f), D3DXVECTOR3(BG_RIGHT, 1.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitMiddleTank(void)
{
	for (int i = 0; i < MIDDLETANK_TEXTURE_NUM; i++)
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
void UpdateMiddleTank(void)
{
	int i;
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	//セッターを更新
	for (i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		if (setter[i].active)
		{
			setter[i].pos.y += MIDDLETANK_SCROLLSPEED;
			setter[i].targetPos.y += MIDDLETANK_SCROLLSPEED;
			if (setter[i].cntFrame % MIDDLETANK_SETTER_INTERBAL == 0)
			{
				SetMiddleTank(setter[i].pos, setter[i].targetPos);
				setter[i].cntSet++;

				if (setter[i].cntSet >= MIDDLETANK_SETTER_SETMAX)
				{
					setter[i].active = false;
				}
			}
			setter[i].cntFrame++;
		}
	}

	//タンクを更新
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
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
			AddScore(MIDDLETANK_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			SetGroundFire(ptr->pos, ptr->rot);
			continue;
		}

		//座標の更新
		ptr->pos += ptr->moveDir * MIDDLETANK_MOVE_SPEED;
		ptr->pos.y += MIDDLETANK_SCROLLSPEED;

		//座標が画面外であれば非アクティブへ
		if (ptr->state != INIT)
		{
			if (ptr->pos.x + MIDDLETANK_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - MIDDLETANK_TEXTURE_SIZE_X > BG_RIGHT
				|| ptr->pos.y + MIDDLETANK_TEXTURE_SIZE_Y < 0.0f || ptr->pos.y - MIDDLETANK_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
			{
				ptr->active = false;
			}
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % MIDDLETANK_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % MIDDLETANK_ANIM_NUM;
		}

		//砲塔を回転
		if (ptr->state != ATTACK)
		{
			ptr->turretRot += RotateToTarget(&ptr->pos, &player->pos, ptr->turretRot, MIDDLETANK_TURRET_ROTANGLE);
		}

		//行動処理
		ActionMiddleTank(ptr);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawMiddleTank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ミドルタンクの本体を描画
	pDevice->SetTexture(0, texture[BODY]);
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexMiddleTank(ptr);

			// テクスチャ座標を設定
			SetTextureMiddleTank(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//ミドルタンクの砲塔を描画
	ptr = GetMiddleTankAdr(0);
	pDevice->SetTexture(0, texture[TURRET]);
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetTurretVertex(ptr);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(MIDDLETANK_BB_LEFT, MIDDLETANK_BB_TOP, 0.0f), MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexMiddleTank(void)
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
void SetTextureMiddleTank(int cntPattern, MIDDLETANK *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % MIDDLETANK_TEXTURE_DIVIDE_X;
	int y = cntPattern / MIDDLETANK_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / MIDDLETANK_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / MIDDLETANK_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定(本体用)
******************************************************************************/
void SetVertexMiddleTank(MIDDLETANK *ptr)
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
void SetTurretVertex(MIDDLETANK *ptr)
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
//関数名	：MIDDLETANK *GetMiddleTankAdr(int pno)
//引数		：int pno：取得したいミドルタンクのインデックス
//戻り値	：正常な引数：ミドルタンクのアドレス	異常な引数：NULL
//説明		：ミドルタンクのアドレス取得関数（メインショット）
******************************************************************************/
MIDDLETANK *GetMiddleTankAdr(int pno)
{
	if (pno < MIDDLETANK_MAX)
	{
		return &middleTank[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//引数1		：D3DXVECTOR3 initPos			：出現座標
//引数2		：D3DXVECTOR3 targetPos			：目標座標
//戻り値	：void
//説明		：ミドルタンクのセット関数
******************************************************************************/
void SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i;
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
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
			ptr->hp = MIDDLETANK_INITHP;
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
//関数名	：void ActionMiddleTank(MIDDLETANK *ptr)
//引数1		：MIDDLETANK *ptr		：行動を行うミドルタンクへのポインタ
//戻り値	：void
//説明		：ミドルタンクの行動処理
******************************************************************************/
void ActionMiddleTank(MIDDLETANK *ptr)
{
	ptr->countFrame++;

	switch (ptr->state)
	{
		//出現直後の状態
	case INIT:
		//出現してから一定時間経過したらATTACK状態へ遷移
		if (ptr->countFrame >= MIDDLETANK_INITFRAME)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//攻撃状態
	case ATTACK:
		if (ptr->countFrame % MIDDLETANK_ATTACK_INTERBAL == 0)
		{
			//攻撃処理
			AttackMiddleTank(ptr);
		}

		//攻撃回数が最大へ到達していたら状態遷移
		if (ptr->attackNum >= MIDDLETANK_ATTACK_MAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//攻撃時間が終わっていれば待機状態へ遷移
		else if (ptr->countFrame >= atkFrame)
		{
			ptr->countFrame = 0;
			ptr->attackNum++;
			ptr->state = WAIT;
		}
		break;

		//待機状態
	case WAIT:
		//一定時間経過していたら攻撃状態へ遷移
		if (ptr->countFrame > MIDDLETANK_WAIT_FRAME)
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
//関数名	：void AttackMiddleTank(MIDDLETANK *ptr)
//引数1		：MIDDLETANK *ptr		：攻撃を行うミドルタンクへのポインタ
//戻り値	：void
//説明		：ミドルタンクの攻撃処理
******************************************************************************/
void AttackMiddleTank(MIDDLETANK *ptr)
{
	ENEMYATTACK param = bulletParam1;
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	float shotAngle = ptr->turretRot + RADIAN(90);

	//発射位置の計算
	shotPos.x = ptr->pos.x + cosf(shotAngle) * MIDDLETANK_SHOTPOS_Y;
	shotPos.y = ptr->pos.y + sinf(shotAngle) * MIDDLETANK_SHOTPOS_Y;

	//パラメータを設定しバレットを発射
	param.pos = shotPos;
	param.angle = shotAngle;
	FireWayBullet(&param, ptr->shotWay, MIDDLETANK_ATTACK_RANGE, &ptr->hp);
}

/******************************************************************************
//関数名	：void CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
//引数1		：D3DXVECTOR3 initPos			：出現座標
//引数2		：D3DXVECTOR3 targetPos			：目標座標
//戻り値	：void
//説明		：ミドルタンクのセッターを設定する関数
******************************************************************************/
void CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
{
	for (int i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		if (!setter[i].active)
		{
			setter[i].active = true;
			setter[i].pos = initPos;
			setter[i].targetPos = target;
			setter[i].cntFrame = 0;
			setter[i].cntSet = 0;
			return;
		}
	}
}