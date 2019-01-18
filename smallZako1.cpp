//=============================================================================
//
// スモールザコ1処理 [smallZako1.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SMALLZAKO1_INITHP					(10)				//HP初期値
#define SMALLZAKO1_SCORE					(100)				//撃墜時のスコア
#define SMALLZAKO1_INITSPEED				(9.5f)				//スピード初期値

#define SMALLZAKO1_BULLET_SPEED				(4.5f)				//バレットのスピード

#define SMALLZAKO1_INIT_FRAME				(15)				//INIT状態から次へ遷移する時間

#define SMALLZAKO1_HOMING_LENGTH			(150*150)			//追尾を止めるプレイヤーとの距離
#define SMALLZAKO1_HOMING_ANGLE				(RADIAN(2.0f))		//追尾時の最小回転角度

#define SMALLZAKO1_SHADOW_OFFSETPOS			(D3DXVECTOR3(50.0f, 100.0f, 0.0f)) //影のオフセット座標

#define SMALLZAKO1_SHOT_WAY					(12)					//撃ち返し弾WAY数

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexSmallZako1(void);							//頂点作成関数
void SetTextureSmallZako1(int cntPattern, SMALLZAKO1 *ptr);	// テクスチャ座標の計算処理
void SetVertexSmallZako1(SMALLZAKO1 *ptr);					// 頂点の計算処理
void SetShadowSmallZako1(SMALLZAKO1 *ptr);					//影用の設定処理
void ActionSmallZako1(SMALLZAKO1 *ptr);						//スモールザコ1の行動処理

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
	HOMING,
	NOT_HOMING
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLZAKO1_TEXTURE_NUM];		// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];							//頂点情報格納ワーク

static float angle;												//ポリゴンの半径
static float radius;											//ポリゴンの頂点角度

SMALLZAKO1 smallZako1[SMALLZAKO1_MAX];							//スモールザコ1配列

static ENEMYATTACK bulletParam = {
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	SMALLZAKO1_BULLET_SPEED,
	0.0f
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSmallZako1(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO1* ptr = GetSmallZako1Adr(0);
	int i;

	angle = atan2f(SMALLZAKO1_TEXTURE_SIZE_Y, SMALLZAKO1_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SMALLZAKO1_TEXTURE_SIZE_X, SMALLZAKO1_TEXTURE_SIZE_Y));

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLZAKO1_INITPOS_X, SMALLZAKO1_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->hp = SMALLZAKO1_INITHP;
		ptr->state = INIT;
		ptr->speed = SMALLZAKO1_INITSPEED;
		ptr->moveDir = VECTOR3_UP * -1;
		ptr->cntFrame = 0;

		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexSmallZako1();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[SHADOW] = CreateTextureFromFile((LPSTR)SMALLZAKO1_SHADOWTEX_NAME, pDevice);
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLZAKO1_TEXTURE_NAME, pDevice);
	}
	//SetSmallZako1(D3DXVECTOR3(BG_LEFT + 200.0f, 0.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSmallZako1(void)
{
	for (int i = 0; i < SMALLZAKO1_TEXTURE_NUM; i++)
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
void UpdateSmallZako1(void)
{
	int i;
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);
	PLAYER *player = GetPlayerAdr(0);
	ENEMYATTACK param = bulletParam;

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
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
			AddScore(SMALLZAKO1_SCORE);

			//撃ち返し弾を発生（難易度：HARD以上）
			D3DXVECTOR3 tmp = player->pos - ptr->pos;
			float distSq = D3DXVec3Length(&tmp);
			if (GetDifficulty() >= DIFFICULTY_HARD && distSq > 100 * 100)
			{
				param.angle = SnipeTarget(&ptr->pos, &player->pos);
				param.pos = ptr->pos;
				FireCircleBullet(&param, SMALLZAKO1_SHOT_WAY, &ptr->hp);
				//SetEnemyBullet(&param, &ptr->hp);
			}
			continue;
		}

		//アニメーション
		ptr->countAnim++;
		if (ptr->countAnim % SMALLZAKO1_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLZAKO1_ANIM_NUM;
		}

		//行動処理
		ActionSmallZako1(ptr);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawSmallZako1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//影を描画
	pDevice->SetTexture(0, texture[SHADOW]);
	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標、テクスチャ座標を設定
			SetShadowSmallZako1(ptr);

			//ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//本体を描画
	pDevice->SetTexture(0, texture[BODY]);
	ptr = GetSmallZako1Adr(0);
	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexSmallZako1(ptr);

			// テクスチャ座標を設定
			SetTextureSmallZako1(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLZAKO1_BB_LEFT, SMALLZAKO1_BB_TOP, 0.0f), SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexSmallZako1(void)
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
void SetTextureSmallZako1(int cntPattern, SMALLZAKO1 *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % SMALLZAKO1_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLZAKO1_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLZAKO1_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLZAKO1_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexSmallZako1(SMALLZAKO1 *ptr)
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
void SetShadowSmallZako1(SMALLZAKO1 *ptr)
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
		vertexWk[i].vtx += SMALLZAKO1_SHADOW_OFFSETPOS;
	}

	//テクスチャ座標を設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//関数名	：SMALLZAKO1 *GetSmallZako1Adr(int pno)
//引数		：int pno：取得したいスモールザコ1のインデックス
//戻り値	：正常な引数：スモールザコ1のアドレス	異常な引数：NULL
//説明		：スモールザコ1のアドレス取得関数
******************************************************************************/
SMALLZAKO1 *GetSmallZako1Adr(int pno)
{
	if (pno < SMALLZAKO1_MAX)
	{
		return &smallZako1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void SetSmallZako1(D3DXVECTOR3 initPos)
//引数		：D3DXVECTOR3 initPos	：出現座標
//戻り値	：void
//説明		：スモールザコ1のセット関数
******************************************************************************/
void SetSmallZako1(D3DXVECTOR3 initPos)
{
	int i;
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		//非アクティブのスモールザコ1がいればセット
		if (!ptr->active)
		{
			//各パラメータを設定
			ptr->pos = initPos;
			ptr->rot.z = 0.0f;
			ptr->hp = SMALLZAKO1_INITHP;
			ptr->moveDir = VECTOR3_UP * -1;
			ptr->speed = SMALLZAKO1_INITSPEED;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->cntFrame = 0;
			ptr->state = INIT;

			//セットしリターン
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//関数名	：void ActionSmallZako1(SMALLZAKO1 *ptr)
//引数		：SMALLZAKO11 *ptr	：行動するエネミーへのポインタ
//戻り値	：void
//説明		：スモールザコ1の行動処理
******************************************************************************/
void ActionSmallZako1(SMALLZAKO1 *ptr)
{
	PLAYER *player = GetPlayerAdr(0);
	float lengthSq = 99999.9f;

	ptr->cntFrame++;

	switch (ptr->state)
	{
	case INIT:
		//進行方向に向かって直進
		ptr->pos += ptr->moveDir * ptr->speed;

		//一定時間経過していたら次の状態へ遷移
		if (ptr->cntFrame > SMALLZAKO1_INIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = HOMING;
		}
		break;

	case HOMING:
		//プレイヤーが非アクティブであれば次の状態へ遷移
		if (!player->active)
		{
			ptr->state = NOT_HOMING;
			ptr->cntFrame = 0;
		}

		//プレイヤーとの距離が一定以下なら次の状態へ遷移
		lengthSq = D3DXVec3LengthSq(&(player->pos - ptr->pos));
		if (lengthSq <= SMALLZAKO1_HOMING_LENGTH)
		{
			ptr->state = NOT_HOMING;
			ptr->cntFrame = 0;
		}
		//そうでないならプレイヤーを追尾
		else
		{
			ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLZAKO1_HOMING_ANGLE);
			ptr->moveDir = D3DXVECTOR3(cosf(ptr->rot.z + RADIAN(90)), sinf(ptr->rot.z + RADIAN(90)), 0.0f);
		}

	case NOT_HOMING:
		//進行方向に向かって直進
		ptr->pos += ptr->moveDir * ptr->speed;

		//更新後の座標が画面外であれば非アクティブに
		if (ptr->pos.x - SMALLZAKO1_TEXTURE_SIZE_X > BG_RIGHT || ptr->pos.x + SMALLZAKO1_TEXTURE_SIZE_X < BG_LEFT
			|| ptr->pos.y - SMALLZAKO1_TEXTURE_SIZE_Y > SCREEN_HEIGHT || ptr->pos.y + SMALLZAKO1_TEXTURE_SIZE_Y < 0.0f)
		{
			ptr->active = false;
		}

		break;
	}
}