//=============================================================================
//
// スモールプロペラ処理 [smallPropeller.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SMALLPROPELLER_INITHP					(70)				//初期体力
#define SMALLPROPELLER_BULLET_SPEED				(4.5f)				//バレットの速度
#define SMALLPROPELLER_ROTANGLE					(RADIAN(5.0f))		//プレイヤーの方へ向く際の角度
#define SMALLPROPELLER_MOVESPEED				(7.5f)				//移動スピード

#define SMALLPROPELLER_ATTACKMAX				(6)					//攻撃の最大回数
#define SMALLPROPELLER_ATTACKINTERBAL			(5)					//攻撃のインターバル
#define SMALLPROPELLER_ATTACKFRAMEEASY			(SMALLPROPELLER_ATTACKINTERBAL*2) //攻撃時間
#define SMALLPROPELLER_ATTACKFRAMENORMAL		(SMALLPROPELLER_ATTACKINTERBAL*5) //攻撃時間
#define SMALLPROPELLER_ATTACKFRAMEHARD			(SMALLPROPELLER_ATTACKINTERBAL*8) //攻撃時間

#define SMALLPROPELLER_WAIT						(60)				//待機時間
#define SMALLPROPELLER_WAITHARD					(30)				//待機時間（HARD）	

#define SMALLPROPELLER_SHADOW_OFFSETPOS			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))	//影のオフセット座標

#define SMALLPROPELLER_SCORE					(500)				//撃墜時のスコア

#define SMALLPROPELLER_ACCELFRAME				(120)				//逃げるときに加速する時間

#define SMALLPROPELLER_ATTACKPOS_X				(25.0f)				//バレット発射位置のX座標
#define SMALLPROPELLER_ATTACKPOS_Y				(50.0f)				//バレット発射位置のY座標

#define SMALLPROPELLER_SHOT_ANGLE				(80)				//WAYショット角度
#define SMALLPROPELLER_SHOT_WAY					(2)					//WAYショットWAY数
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexSmallPropeller(void);								//頂点作成関数
void SetTextureSmallPropeller(int cntPattern, SMALLPROPELLER *ptr);	// テクスチャ座標の計算処理
void SetVertexSmallPropeller(SMALLPROPELLER *ptr);					// 頂点の計算処理
void SetShadowSmallPropeller(SMALLPROPELLER *ptr);					//影用の頂点設定処理
void SmallPropellerAction(SMALLPROPELLER *ptr);						//行動処理
void SmallPropellerAttack(SMALLPROPELLER *ptr);						//攻撃処理

/*****************************************************************************
構造体定義
*****************************************************************************/
//エネミーの行動状態を示す列挙体
enum
{
	MOVE,
	WAIT,
	ATTACK,
	ESCAPE
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLPROPELLER_TEXTURE_NUM];	// テクスチャへのポインタ

static SMALLPROPELLER sPropeller[SMALLPROPELLER_MAX];			//スモールプロペラ配列

static VERTEX_2D vertexWk[NUM_VERTEX];							//頂点情報格納ワーク
static int atkFrame;											//攻撃時間

static ENEMYATTACK bulletParam2 =								//攻撃用のバレットパラメータ
{
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	SMALLPROPELLER_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSmallPropeller(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLPROPELLER* ptr = GetSmallPropellerAdr(0);
	int i;

	//難易度によって攻撃時間を分岐
	int difficulty = GetDifficulty();
	atkFrame = (difficulty == DIFFICULTY_EASY) ? SMALLPROPELLER_ATTACKFRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLPROPELLER_ATTACKFRAMENORMAL : SMALLPROPELLER_ATTACKFRAMEHARD;

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLPROPELLER_INITPOS_X, SMALLPROPELLER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SMALLPROPELLER_TEXTURE_SIZE_Y, SMALLPROPELLER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SMALLPROPELLER_TEXTURE_SIZE_X, SMALLPROPELLER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->shotPosAngle = atan2f(SMALLPROPELLER_ATTACKPOS_Y, SMALLPROPELLER_ATTACKPOS_X);
		ptr->shotPosRadius = D3DXVec2Length(&D3DXVECTOR2(SMALLPROPELLER_ATTACKPOS_X, SMALLPROPELLER_ATTACKPOS_Y));

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexSmallPropeller();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[0] = CreateTextureFromFile((LPSTR)SMALLPROPELLER_SHADOWTEX_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)SMALLPROPELLER_TEXTURE_NAME, pDevice);
	}
	//SetSmallPropeller(D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSmallPropeller(void)
{
	for (int i = 0; i < SMALLPROPELLER_TEXTURE_NUM; i++)
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
void UpdateSmallPropeller(void)
{
	int i;
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//非アクティブであれば次のエネミーへ
		if (!ptr->active)
		{
			continue;
		}

		//撃墜判定
		if (ptr->hp <= 0)
		{
			//撃墜処理
			ptr->active = false;
			AddScore(SMALLPROPELLER_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % SMALLPROPELLER_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLPROPELLER_ANIM_NUM;
		}

		//行動処理
		SmallPropellerAction(ptr);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawSmallPropeller(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 影を描画
	pDevice->SetTexture(0, texture[0]);
	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//アクティブであれば影を描画
		if (ptr->active)
		{
			SetShadowSmallPropeller(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//スモールプロペラを描画
	pDevice->SetTexture(0, texture[1]);
	ptr = GetSmallPropellerAdr(0);
	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexSmallPropeller(ptr);

			// テクスチャ座標を設定
			SetTextureSmallPropeller(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLPROPELLER_BB_LEFT, SMALLPROPELLER_BB_TOP, 0.0f), SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexSmallPropeller(void)
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
void SetTextureSmallPropeller(int cntPattern, SMALLPROPELLER *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % SMALLPROPELLER_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLPROPELLER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLPROPELLER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLPROPELLER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexSmallPropeller(SMALLPROPELLER *ptr)
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
影用の頂点設定
******************************************************************************/
void SetShadowSmallPropeller(SMALLPROPELLER *ptr)
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

	//影用に頂点をずらす
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += SMALLPROPELLER_SHADOW_OFFSETPOS;
	}

	//テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//関数名	：SMALLPROPELLER *GetSmallPropellerAdr(int pno)
//引数		：int pno：取得したいスモールプロペラのインデックス
//戻り値	：正常な引数：スモールプロペラのアドレス	異常な引数：NULL
//説明		：スモールプロペラのアドレス取得関数（メインショット）
******************************************************************************/
SMALLPROPELLER *GetSmallPropellerAdr(int pno)
{
	if (pno < SMALLPROPELLER_MAX)
	{
		return &sPropeller[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//引数1		：D3DXVECTOR3 initPos		：出現座標
//引数2		：D3DXVECTOR3 targetPos		：目標座標
//戻り値	：void
//説明		：スモールプロペラを出現させる関数
******************************************************************************/
void SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i = 0;
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//非アクティブのエネミーであれば出現させる
		if (!ptr->active)
		{
			//各座標の設定
			ptr->pos = initPos;
			ptr->targetPos = targetPos;

			//各種パラメータの初期化
			ptr->hp = SMALLPROPELLER_INITHP;
			ptr->rot = VECTOR3_ZERO;
			ptr->patternAnim = 0;
			ptr->countAnim = 0;

			//アクティブにしてリターン
			ptr->state = MOVE;
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//関数名	：void SmallPropellerAction(SMALLPROPELLER *ptr)
//引数1		：SMALLPROPELLER *ptr	：行動するスモールプロペラへのポインタ
//戻り値	：void
//説明		：スモールプロペラの行動処理
******************************************************************************/
void SmallPropellerAction(SMALLPROPELLER *ptr)
{
	bool flgMove = false;
	PLAYER *player = GetPlayerAdr(0);
	int wait = (GetDifficulty() == DIFFICULTY_HARD) ? SMALLPROPELLER_WAITHARD : SMALLPROPELLER_WAIT;
	ptr->countFrame++;

	//プレイヤーの方へ向く
	ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLPROPELLER_ROTANGLE);

	switch (ptr->state)
	{
		//目標座標へ向かって移動する状態
	case MOVE:		
		//移動
		flgMove = MoveToTarget(&ptr->pos, &ptr->targetPos, SMALLPROPELLER_MOVESPEED);
		if (flgMove)
		{
			//目標座標へ到達していたら次の状態へ遷移
			ptr->countFrame = 0;
			ptr->attackNum = 0;
			ptr->state = WAIT;
		}
		break;

		//攻撃後の待機状態
	case WAIT:
		//待機時間を経過していたら攻撃状態へ遷移
		if (ptr->countFrame >= wait)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//攻撃する状態
	case ATTACK:
		if (ptr->countFrame % SMALLPROPELLER_ATTACKINTERBAL == 0)
		{
			//攻撃処理
			SmallPropellerAttack(ptr);
		}

		//攻撃回数が最大に到達していたら逃げる状態へ遷移
		if (ptr->attackNum >= SMALLPROPELLER_ATTACKMAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//攻撃時間を経過していたら待機状態へ遷移
		else if (ptr->countFrame >= atkFrame)
		{
			ptr->attackNum++;					//攻撃回数をカウント
			ptr->countFrame = 0;
			ptr->state = WAIT;
		}
		break;


		//攻撃を終了して逃げる状態
	case ESCAPE:
		//移動
		StartAccel(&ptr->pos, &VECTOR3_UP, SMALLPROPELLER_MOVESPEED, SMALLPROPELLER_ACCELFRAME, ptr->countFrame);

		//画面外へ出ていたら非アクティブに
		if (ptr->pos.y <= -SMALLPROPELLER_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//関数名	：void SmallPropellerAttack(SMALLPROPELLER *ptr)
//引数1		：SMALLPROPELLER *ptr	：攻撃するスモールプロペラへのポインタ
//戻り値	：void
//説明		：スモールプロペラの攻撃処理
******************************************************************************/
void SmallPropellerAttack(SMALLPROPELLER *ptr)
{
	ENEMYATTACK param = bulletParam2;
	PLAYER *player = GetPlayerAdr(0);
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	int difficulty = GetDifficulty();

	//左の発射位置から発射
	shotPos.x = ptr->pos.x - cosf(ptr->shotPosAngle - ptr->rot.z) * ptr->shotPosRadius;
	shotPos.y = ptr->pos.y + sinf(ptr->shotPosAngle - ptr->rot.z) * ptr->shotPosRadius;
	param.pos = shotPos;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	SetEnemyBullet(&param, &ptr->hp);

	//難易度HARDならWAY弾追加
	if (difficulty == DIFFICULTY_HARD)
	{
		FireWayBullet(&param, SMALLPROPELLER_SHOT_WAY, SMALLPROPELLER_SHOT_ANGLE, &ptr->hp);
	}

	//右の発射位置から発射
	shotPos.x = ptr->pos.x + cosf(ptr->shotPosAngle + ptr->rot.z) * ptr->shotPosRadius;
	shotPos.y = ptr->pos.y + sinf(ptr->shotPosAngle + ptr->rot.z) * ptr->shotPosRadius;
	param.pos = shotPos;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	SetEnemyBullet(&param, &ptr->hp);

	//難易度HARDならWAY弾追加
	if (difficulty == DIFFICULTY_HARD)
	{
		FireWayBullet(&param, SMALLPROPELLER_SHOT_WAY, SMALLPROPELLER_SHOT_ANGLE, &ptr->hp);
	}
}