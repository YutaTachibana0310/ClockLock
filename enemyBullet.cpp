//=============================================================================
//
// エネミーバレット処理 [enemyBullet.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "bg.h"
#include "myLibrary.h"
#include "scoreItem.h"
#include "EnemyBulletExplosion.h"

#ifdef _DEBUG
#include "player.h"
#endif

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define ENEMYBULLET_MAGNI		((ptr->type < ORANGE_ROTATE) ? magniGreen : magniOrange)	//緑色とオレンジ色で速度倍率を使い分けるマクロ
#define ENEMYBULLET_SLOW		(0.1f)							//遅いときの速度倍率
#define ENEMYBULLET_FAST		(1.0f)							//早いときの速度倍率
#define ENEMYBULLET_SLOWBOTH	(0.0f)							//両方遅いときの速度倍率

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexEnemyBullet(void);							// 頂点作成関数
void SetTextureEnemyBullet(int cntPattern, ENEMYBULLET *ptr);	// テクスチャ座標の計算処理
void SetVertexEnemyBullet(ENEMYBULLET *ptr);					// 頂点の計算処理
void DecideSpeedMagni(float *magniO, float *magniG);			// 速度倍率の決定関数


/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[ENEMYBULLET_NUM_TEXTURE];			// エネミーバレットのテクスチャ

ENEMYBULLET				bullet[ENEMYBULLET_KIND_MAX][ENEMYBULLET_NUM_MAX];	//エネミーバレット配列

static VERTEX_2D		vertexWk[NUM_VERTEX];								//頂点情報格納ワーク
SPEEDTYPE				speedType;											//エネミーバレットの速度のモード

static int interbal = 0;

#ifdef _DEBUG
static int cntBullet;														//アクティブなエネミーバレットの数
#endif

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitEnemyBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET* ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;
	float x = ENEMYBULLET_TEXTURE_SIZE_X / 2.0f;
	float y = ENEMYBULLET_TEXTURE_SIZE_Y / 2.0f;

#ifdef _DEBUG
	cntBullet = 0;
#endif // _DEBUG

	for (i = 0; i < ENEMYBULLET_KIND_MAX*ENEMYBULLET_NUM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYBULLET_INITPOS_X, ENEMYBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->angle = atan2f(y, x);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(x, y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->type = (BULLET_TYPE)(i / ENEMYBULLET_NUM_MAX);
		ptr->velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->active = false;
	}
	// 頂点情報の作成
	MakeVertexEnemyBullet();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[0] = CreateTextureFromFile((LPSTR)ENEMYBULLET_TEXTURE_MAIN, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)ENEMYBULLET_TEXTURE_LUMINE, pDevice);
	}
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnemyBullet(void)
{
	if (texture[0] != NULL)
	{	// テクスチャの開放
		texture[0]->Release();
		texture[0] = NULL;
	}

	if (texture[1] != NULL)
	{
		texture[1]->Release();
		texture[1] = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateEnemyBullet(void)
{
	ENEMYBULLET *ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;
	float magniOrange, magniGreen;							//それぞれの弾の速度倍率^

	DecideSpeedMagni(&magniOrange, &magniGreen);			//弾の速度倍率を決定

#ifdef _DEBUG
	
	PLAYER *player = GetPlayerAdr(0);
	if (speedType != SLOW_BOTH)
	{
		interbal++;
	}
	if (interbal % 30 == 0)
	{
		ENEMYATTACK param1 = { GREEN_ROTATE, D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 5.0f, RADIAN(10) };
		ENEMYATTACK param2 = { ORANGE_ROTATE, D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 5.0f, RADIAN(32.5f) };
		//param1.angle = SnipeTarget(&D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), &(player->pos));
		//param2.angle = SnipeTarget(&D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), &(player->pos));
		//SetEnemyBullet(&param1);
		//FireWayBullet(&param1, 3, 120.0f);
		//FireCircleBullet(&param1, 8);
		//FireCircleBullet(&param2, 8);
	}
	
#endif

	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//親が死んでいるかどうか
			if (ptr->parentHp != NULL && ptr->type < ORANGE_ROTATE)
			{
				if (*(ptr->parentHp) <= 0)
				{
					if (speedType == SLOW_GREEN)
					{
						DisableEnemyBullet(ptr);
						SetScoreItem(ptr->pos, 10.0f, 1);
					}
					else
					{
						ptr->parentHp = NULL;
					}
				}
			}

			//座標の更新
			ptr->pos += ptr->velocity * ENEMYBULLET_MAGNI;
			if (ptr->pos.y < -ENEMYBULLET_TEXTURE_SIZE_Y || ptr->pos.y > SCREEN_HEIGHT)
			{
#ifdef _DEBUG
				if (ptr->active)
				{
					cntBullet--;
				}
#endif
				ptr->active = false;
				ptr->parentHp = NULL;
			}
			else if (ptr->pos.x < BG_POS_X - ENEMYBULLET_TEXTURE_SIZE_X / 2 || ptr->pos.x > BG_POS_X + BG_TEXTURESIZE_X + ENEMYBULLET_BB_WIDTH / 2)
			{
#ifdef _DEBUG
				if (ptr->active)
				{
					cntBullet--;
				}
#endif
				ptr->active = false;
				ptr->parentHp = NULL;
			}

			// アニメーション
			ptr->countAnim++;

			// アニメーションWaitチェック
			if (ptr->countAnim % TIMEENEMYBULLET_ANIMATION == 0)
			{
				// パターンの切り替え
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_ENEMYBULLET_NUM;
			}
		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET *ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//エネミーバレットの光を描画
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEnemyBullet(ptr);

			// テクスチャ座標を設定
			SetTextureEnemyBullet(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//エネミーバレット本体を描画
	ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	pDevice->SetTexture(0, texture[0]);
	for (i = GREEN_ROTATE; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexEnemyBullet(ptr);

			// テクスチャ座標を設定
			SetTextureEnemyBullet(ptr->patternAnim, ptr);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos, ENEMYBULLET_BB_WIDTH, ENEMYBULLET_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexEnemyBullet()
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
void SetTextureEnemyBullet(int cntPattern, ENEMYBULLET *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % ENEMYBULLET_TEXTURE_DIVIDE_X;
	int y = ptr->type;
	float sizeX = 1.0f / ENEMYBULLET_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYBULLET_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexEnemyBullet(ENEMYBULLET *ptr)
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
//関数名	：ENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno) {
//引数1		：BULLET_TYPE type	：取得したいバレットのタイプ
//引数2		：int pno			：取得したいエネミーバレットのインデックス
//戻り値	：正常な引数：エネミーバレットのアドレス	異常な引数：NULL
//説明		：エネミーバレットのアドレス取得関数
******************************************************************************/
ENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno) {
	if (pno < ENEMYBULLET_NUM_MAX && type < ENEMYBULLET_KIND_MAX)
	{
		return &bullet[type][pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：ENEMYBULLET *SetEnemyBullet(BULLET_PARAM *param)
//引数1		：BULLET_PARAM *param	：発射したいバレットへの攻撃パラメータ
//戻り値	：セット成功時：エネミーバレットのアドレス	セット異常時：NULL
//説明		：エネミーバレットをゲーム内にセットする関数
******************************************************************************/
ENEMYBULLET *SetEnemyBullet(ENEMYATTACK *param, int *parentHp)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr(param->type, 0);
	float angle = param->angle;

	for (i = 0; i < ENEMYBULLET_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = param->pos;
			ptr->velocity = param->speed * D3DXVECTOR3(cosf(angle), sinf(angle), 0.0f);
			ptr->rot.z = angle + ENEMYBULLET_DAFAULT_ANGLE;
			ptr->active = true;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->parentHp = parentHp;
#ifdef _DEBUG
			cntBullet++;
#endif
			return ptr;
		}
	}
	return NULL;
}

/******************************************************************************
//関数名	：void SetSpeedType(SPEEDTYPE type)
//引数1		：SPEEDTYPE type	：セットしたいスピードタイプ
//戻り値	：void
//説明		：スピードタイプをセットする関数
******************************************************************************/
void SetSpeedType(SPEEDTYPE type)
{
	speedType = type;
}

/******************************************************************************
//関数名	：void GetSpeedType(void)
//引数1		：void
//戻り値	：void
//説明		：スピードタイプを取得する関数
******************************************************************************/
SPEEDTYPE GetSpeedType(void)
{
	return speedType;
}

/******************************************************************************
//関数名	：void DecideSpeedMode(int *magniO, int *magniG)
//引数1		：int *magniO	：オレンジの弾の速度倍率を格納するポインタ
//引数2		：int *magniG	：緑の弾の速度倍率を格納するポインタ
//戻り値	：void
//説明		：エネミーバレットの速度倍率を決定する関数
******************************************************************************/
void DecideSpeedMagni(float *magniO, float *magniG)
{
	//オレンジが遅く、緑が速い
	if (speedType == SLOW_ORANGE)
	{
		*magniO = ENEMYBULLET_SLOW;
		*magniG = ENEMYBULLET_FAST;
	}
	//オレンジが速く、緑が遅い
	else if (speedType == SLOW_GREEN)
	{
		*magniO = ENEMYBULLET_FAST;
		*magniG = ENEMYBULLET_SLOW;
	}
	//両方遅い
	else if (speedType == SLOW_BOTH)
	{
		*magniO = ENEMYBULLET_SLOWBOTH;
		*magniG = ENEMYBULLET_SLOWBOTH;
	}
	//両方速い
	else if (speedType == FAST_BOTH)
	{
		*magniO = ENEMYBULLET_FAST;
		*magniG = ENEMYBULLET_FAST;
	}
	//デフォルト
	else
	{
		*magniO = 1.0f;
		*magniG = 1.0f;
	}
}

/******************************************************************************
//関数名	：void FireCircleBullet(BULLET_PARAM *param, int way)
//引数1		：BULLET_PARAM *param	：攻撃のパラメータが保存された構造体へのポインタ
//引数2		：int way				：way数
//戻り値	：void
//説明		：円形に沿って弾をway発撃つ関数
******************************************************************************/
void FireCircleBullet(ENEMYATTACK *param, int way, int *parentHp)
{
	int i;
	float tmpAngl = param->angle;

	for (i = 0; i < 360 / way; i++)
	{
		SetEnemyBullet(param, parentHp);
		param->angle += RADIAN(360.0f / way);
	}
	param->angle = tmpAngl;
}

/******************************************************************************
//関数名	：void FireWayBullet(BULLET_PARAM *param, int way, float range)
//引数1		：BULLET_PARAM *param	：攻撃のパラメータが保存された構造体へのポインタ
//引数2		：int way				：way数
//引数3		：float range			：WAY弾が広がる角度
//戻り値	：void
//説明		：扇状に弾をway発撃つ関数
******************************************************************************/
void FireWayBullet(ENEMYATTACK *param, int way, float range, int *parentHp)
{
	int i;
	float tmpAngl = param->angle;

	param->angle -= RADIAN(range / 2.0f);

	for (i = 0; i < way; i++)
	{
		SetEnemyBullet(param, parentHp);
		param->angle += RADIAN(range / (way - 1));
	}
	param->angle = tmpAngl;
}

/******************************************************************************
//関数名	：void DisableEnemyBullet(ENEMYBULLET *ptr)
//引数1		：ENEMYBULLET *ptr	：非アクティブにしたいエネミーバレットへのポインタ
//戻り値	：void
//説明		：指定したエネミーバレットを非アクティブにする関数
******************************************************************************/
void DisableEnemyBullet(ENEMYBULLET *ptr)
{
#ifdef _DEBUG
	if (ptr->active)
	{
		cntBullet--;
	}
#endif

	ptr->active = false;
	ptr->parentHp = NULL;
	//エフェクトをセット
	SetEnemyBulletExplosion(ptr->pos, ptr->type);

}

/******************************************************************************
//関数名	：void DisableAllEnemyBullet(ENEMYBULLET *ptr)
//引数1		：void
//戻り値	：void
//説明		：全てのエネミーバレットを非アクティブにする関数
******************************************************************************/
void DisableAllEnemyBullet(void)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr((BULLET_TYPE)0, 0);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			DisableEnemyBullet(ptr);
		}
	}
}

/******************************************************************************
//関数名	：void ConvertBulletToScoreItem(void)
//引数1		：void
//戻り値	：void
//説明		：全てのエネミーバレットをスコアアイテムに変換する関数
******************************************************************************/
void ConvertBulletToScoreItem(void)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr((BULLET_TYPE)0, 0);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			DisableEnemyBullet(ptr);
			SetScoreItem(ptr->pos, 10.0f, 1);
		}
	}
}

#ifdef _DEBUG
/******************************************************************************
//関数名	：int GetBulletCnt
//引数1		：int		：ゲーム内でアクティブなエネミーバレットの数
//戻り値	：void
//説明		：アクティブなエネミーバレットの数を取得する関数
******************************************************************************/
int GetEnemyBulletCnt(void)
{
	return cntBullet;
}
#endif